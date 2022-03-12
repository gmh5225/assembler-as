#include <iostream>
#include <algorithm>

#include "symbol.hpp"
#include "register.hpp"

SymbolParser::SymbolParser(std::string input) {
    scanner = new Scanner(input);
    symbols = new Symbols;
}

Symbols *SymbolParser::getSymbols() {
    Token token = scanner->getNext();
    
    while (token.type != Eof) {
        if (token.type == Id && token.id_val == ".text") {
            parseText();
        }
        token = scanner->getNext();
    }
    
    return symbols;
}

//
// Adds symbols to the ELF file
//
void SymbolParser::processSymbols(Elf64File *file) {
    for (auto const &x : symbols->locations) {
        bool isGlobal = false;
        if (std::find(symbols->global.begin(), symbols->global.end(), x.first) != symbols->global.end())
            isGlobal = true;
        file->addFunctionSymbol(x.first, x.second, isGlobal);
    }
}

void SymbolParser::parseText() {
    Token token = scanner->getNext();
    while (token.type != Eof) {
        switch (token.type) {
            // Instructions that require further disection
            case Add:
            case Sub:
            case And:
            case Or:
            case Xor:
            case Mov: parseStdInstr(token.type); break;
            
            // Instructions where we know the size right off
            case Syscall: location += 2; break;
            case Ret: location += 1; break;
            
            case Push: {
                token = scanner->getNext();
                if (getRegSize(token.type) != 64) {
                    std::cerr << "Error: Expected 64-bit register with push." << std::endl;
                    return;
                }
                
                location += 1;
            } break;
            
            // Jumps
            case Jmp: {
                //std::cout << "JMP: " << scanner->getLine() << " @ " << location << std::endl;
                symbols->jumps[scanner->getLine()] = location;
                location += 2;
                token = scanner->getNext();
            } break;
            
            // ID value- we found a label
            case Id: {
                std::string name = token.id_val;
                
                // First, check for directives
                if (name == ".global") {
                    token = scanner->getNext();
                    if (token.type != Id) {
                        std::cerr << "Error: Expected name after global." << std::endl;
                        return;
                    }
                    
                    symbols->global.push_back(token.id_val);
                    break;
                }
                
                // Otherwise, we probably have a symbol
                token = scanner->getNext();
                if (token.type != Colon) {
                    std::cerr << "Error: Expected : after label name." << std::endl;
                    return;
                }
                
                symbols->locations[name] = location;
            } break;
        
            default: {
                std::cerr << "Error: Unknown token in text section." << std::endl;
                token.print();
            }
        }
        
        token = scanner->getNext();
    }
}

// Parse MOV instruction
//
// In general:
// mov al/ah, 5 -> size of 2
// mov bl, al -> size of 2
// mov bx, 5 -> size of 4
// mov ax, bx -> size of 3
// mov eax, 10 -> size of 5
// mov eax, ebx -> size of 2
// mov rax, 5 -> size of 7
// mov rax, rbx -> size of 3
//
// Using "mov ax, etc" requires 0x66 prefix
// Using ANY 64-bit register requires 0x48 prefix (REX prefix)
//
void SymbolParser::parseStdInstr(TokenType op) {
    Token token = scanner->getNext();
    int regSize = 0;
    bool destMemory = false;
    bool isDestExt = false;
    
    switch (token.type) {
        case LBrace:
        case DWORD: {
            if (token.type == DWORD) {
                token = scanner->getNext();
                token = scanner->getNext();
                if (token.type != LBrace) {
                    std::cerr << "Error: Expected opening \'[\'." << std::endl;
                    return;
                }
            }
            
            token = scanner->getNext();
            if (getRegSize(token.type) == -1) {
                std::cerr << "Error: Expected base register." << std::endl;
                token.print();
                return;
            }
            
            token = scanner->getNext();
            if (token.type == RBrace) break;
            
            token = scanner->getNext();
            if (token.type != Int32 && getRegSize(token.type) == -1) {
                std::cerr << "Error: Unknown token in address." << std::endl;
                return;
            }
            
            token = scanner->getNext();
            if (token.type != RBrace) {
                std::cerr << "Error: Expected closing brace in memory reference." << std::endl;
                return;
            }
            
            // In most cases, memory moves are the same size unless we are moving an immediate
            destMemory = true;
            location += 3;
        } break;
    
        // Default to a register
        default: {
            regSize = getRegSize(token.type);
            isDestExt = isRegisterExt(token.type);
            if (regSize == -1) {
                std::cerr << "Error: Invalid register in mov." << std::endl;
                return;
            }
        }
    }
    
    // Next token should always be a comma
    token = scanner->getNext();
    if (token.type != Comma) {
        std::cerr << "Error: Invalid syntax in mov-> no comma." << std::endl;
        return;
    }
    
    // Now, the source
    // IMPORTANT: All this needs to be update for extended registers
    token = scanner->getNext();
    switch (token.type) {
        // TODO: Clean this up
        case Eax:
        case Ebx:
        case Ecx:
        case Edx:
        case Esp:
        case Ebp:
        case Esi:
        case Edi:
        case R8d:
        case R9d:
        case R10d:
        case R11d:
        case R12d:
        case R13d:
        case R14d:
        case R15d: {
            if (destMemory) {
                if (isRegisterExt(token.type)) ++location;
                break;
            }
            if (regSize != 32) {
                std::cerr << "Error: Invalid mov. Expected 32-bit destination." << std::endl;
                return;
            }
            
            location += 2;
            if (isDestExt || isRegisterExt(token.type)) ++location;
        } break;
        
        case Rax:
        case Rbx:
        case Rcx:
        case Rdx:
        case Rsp:
        case Rbp:
        case Rsi:
        case Rdi:
        case R8:
        case R9:
        case R10:
        case R11:
        case R12:
        case R13:
        case R14:
        case R15: {
            if (destMemory) {
                ++location;
                break;
            }
            if (regSize != 64) {
                std::cerr << "Error: Invalid mov. Expected 64-bit destination." << std::endl;
                return;
            }
            
            location += 3;
        } break;
        
        case Int32: {
            if (destMemory) {
                location += 4;
            } else if (op == Mov) {
                if (regSize == 8) location += 2;
                else if (regSize == 16) location += 4;
                else if (regSize == 32) location += 5;
                else if (regSize == 64) location += 7;
                
                if (isDestExt && regSize != 64) ++location;
            } else {
                if (regSize == 8) location += 3;
                else if (regSize == 16) location += 3;
                else if (regSize == 32) location += 3;
                else if (regSize == 64) location += 4;
                
                if (isDestExt && regSize != 64) ++location;
            }
        } break;
        
        case LBrace: {
            token = scanner->getNext();
            while (token.type != RBrace) token = scanner->getNext();
            
            // TODO: Update accordingly
            if (regSize == 8) location += 3;
            else if (regSize == 16) location += 4;
            else if (regSize == 32) location += 3;
            else if (regSize == 64) location += 4;
            
            if (isDestExt && regSize != 64) ++location;
        } break;
        
        default: {
            // Check for an 8-bit register-register move
            if (isRegister8(token.type)) {
                if (destMemory) {
                    if (isRegisterExt(token.type)) ++location;
                    break;
                }
                if (regSize != 8) {
                    std::cerr << "Error: Invalid mov. Expected 8-bit destination." << std::endl;
                    return;
                }
                
                location += 2;
                if (isDestExt || isRegisterExt(token.type)) ++location;
                
            // Check for a 16-bit register-register move
            } else if (isRegister16(token.type)) {
                if (destMemory) {
                    ++location;
                    if (isRegisterExt(token.type)) ++location;
                    break;
                }
                if (regSize != 16) {
                    std::cerr << "Error: Invalid mov. Expected 16-bit destination." << std::endl;
                    return;
                }
                
                location += 3;
                if (isDestExt || isRegisterExt(token.type)) ++location;
                
            // Otherwise we have an error
            } else {
                std::cerr << "Error: Invalid mov. Unknown source operand." << std::endl;
            }
        }
    }
}

// Returns size of a register
//
// If -1 -> Invalid register
//
int SymbolParser::getRegSize(TokenType type) {
    switch (type) {
        case Al: case Ah:
        case Cl: case Ch:
        case Dl: case Dh:
        case Bl: case Bh:
        case Spl:
        case Bpl:
        case Sil:
        case Dil:
        case R8b:
        case R9b:
        case R10b:
        case R11b:
        case R12b:
        case R13b:
        case R14b:
        case R15b: return 8;
        
        case Ax:
        case Bx:
        case Cx:
        case Dx:
        case Sp:
        case Bp:
        case Si:
        case Di:
        case R8w:
        case R9w:
        case R10w:
        case R11w:
        case R12w:
        case R13w:
        case R14w:
        case R15w: return 16;
        
        case Eax:
        case Ebx:
        case Ecx:
        case Edx:
        case Esp:
        case Ebp:
        case Esi:
        case Edi:
        case R8d:
        case R9d:
        case R10d:
        case R11d:
        case R12d:
        case R13d:
        case R14d:
        case R15d: return 32;
        
        case Rax:
        case Rbx:
        case Rcx:
        case Rdx:
        case Rsp:
        case Rbp:
        case Rsi:
        case Rdi:
        case R8:
        case R9:
        case R10:
        case R11:
        case R12:
        case R13:
        case R14:
        case R15: return 64;
        
        default: return -1;
    }
    
    return -1;
}

