#include <iostream>
#include <algorithm>

#include "symbol.hpp"

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
            case Mov: parseMov(); break;
            
            // Instructions where we know the size right off
            case Syscall: location += 2; break;
            case Ret: location += 1; break;
            
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
void SymbolParser::parseMov() {
    Token token = scanner->getNext();
    int regSize = 0;
    bool destMemory = false;
    
    switch (token.type) {
        case DWORD: {
            token = scanner->getNext();
            token = scanner->getNext();
            if (token.type != LBrace) {
                std::cerr << "Error: Expected opening \'[\'." << std::endl;
                return;
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
    token = scanner->getNext();
    switch (token.type) {
        case Eax:
        case Ebx:
        case Ecx:
        case Edx:
        case Esp:
        case Ebp:
        case Esi:
        case Edi: {
            if (destMemory) break;
            if (regSize != 32) {
                std::cerr << "Error: Invalid mov. Expected 32-bit destination." << std::endl;
                return;
            }
            
            location += 2;
        } break;
        
        case Int32: {
            if (destMemory) {
                location += 4;
            } else {
                if (regSize == 8) location += 2;
                else if (regSize == 16) location += 4;
                else if (regSize == 32) location += 5;
                else if (regSize == 64) location += 7;
            }
        } break;
        
        case LBrace: {
            token = scanner->getNext();
            while (token.type != RBrace) token = scanner->getNext();
            
            // TODO: Update accordingly
            if (regSize == 8) location += 3;
            else if (regSize == 16) location += 3;
            else if (regSize == 32) location += 3;
            else if (regSize == 64) location += 4;
        } break;
        
        default: {
            std::cerr << "Error: Invalid mov. Unknown source operand." << std::endl;
        }
    }
}

// Returns size of a register
//
// If -1 -> Invalid register
//
int SymbolParser::getRegSize(TokenType type) {
    switch (type) {
        case Eax:
        case Ebx:
        case Ecx:
        case Edx:
        case Esp:
        case Ebp:
        case Esi:
        case Edi: return 32;
        
        case Rax:
        case Rbx:
        case Rcx:
        case Rdx:
        case Rsp:
        case Rbp:
        case Rsi:
        case Rdi: return 64;
        
        default: return -1;
    }
    
    return -1;
}

