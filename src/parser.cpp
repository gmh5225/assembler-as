#include <iostream> // TODO: Remove when done

#include "parser.hpp"
#include "register.hpp"

Parser::Parser(std::string input, Elf64File *file, Symbols *symbols) {
    scanner = new Scanner(input);
    this->file = file;
    this->symbols = symbols;
}

// TODO: We will eventually need data parsing
void Parser::build() {
    Token token = scanner->getNext();
    
    while (token.type != Eof) {
        if (token.type == Id && token.id_val == ".text") {
            parseText();
        }
        token = scanner->getNext();
    }
}

void Parser::parseText() {
    Token token = scanner->getNext();
    
    while (token.type != Eof) {
        switch (token.type) {
            // Complicated instructions
            case Push: buildPush(); break;
            
            case Add:
            case Sub:
            case And:
            case Or:
            case Xor:
            case Mov:
            case Lea: buildStdInstr(token.type); break;
            
            // Jumps
            case Jmp: {
                // Calculate the jump
                int jmp_location = symbols->jumps[scanner->getLine()];
                
                token = scanner->getNext();
                std::string label = token.id_val;
                
                int label_location = symbols->locations[label];
                int destination = label_location - jmp_location - 2;
                
                //std::cout << "JMP: " << jmp_location << " | LBL: " << label_location << std::endl;
                //printf("DEST: %d <%x>\n", destination, destination);
                
                // Encode
                file->addCode8(0xEB);
                file->addCode8(destination);
            } break;
            
            // Calls
            case Call: {
                scanner->getNext();     // Consume function name
                
                file->addCode8(0xE8);
                file->addCode32(0);
            } break;
            
            // Simple instructions
            case Syscall: {
                file->addCode8(0x0F);
                file->addCode8(0x05);
            } break;
            
            case Ret: file->addCode8(0xC3); break;
            
            default: {}
        }
        
        token = scanner->getNext();
    }
}

void Parser::buildPush() {
    Token reg = scanner->getNext();
    
    switch (reg.type) {
        case Rax: file->addCode8(0x50); break;
        case Rcx: file->addCode8(0x51); break;
        case Rdx: file->addCode8(0x52); break;
        case Rbx: file->addCode8(0x53); break;
        case Rsp: file->addCode8(0x54); break;
        case Rbp: file->addCode8(0x55); break;
        case Rsi: file->addCode8(0x56); break;
        case Rdi: file->addCode8(0x57); break;
        
        default: {}
    }
}

void Parser::buildStdInstr(TokenType op) {
    Token dest = scanner->getNext();
    
    // Build memory destinations
    if (dest.type == DWORD || dest.type == LBrace) {
        if (dest.type != LBrace) {
            scanner->getNext();     // consume PTR
            scanner->getNext();     // consume '['
        }
        Token base = scanner->getNext();
        int offset = 1;
        
        Token next = scanner->getNext();
        if (next.type == RBrace) {
            // TODO... a whole other problem
        }
        
        Token offsetToken = scanner->getNext();
        if (isRegister(offsetToken.type)) {
            // TODO
        } else if (offsetToken.type == Int32) {
            offset = offsetToken.i32_val;
            if (next.type == Minus) offset *= -1;
            
            scanner->getNext();                 // Consume ']'
            scanner->getNext();                 // Consume ','
            Token src = scanner->getNext();
            
            // Now, encode
            if (src.type == Int32) {
                file->addCode8(0xC7);
                writeDspOperand(1, base.type, EmptyToken, offset);
                file->addCode32(src.i32_val);
            } else if (isRegister(src.type)) {
                // Prefixes
                if (isRegister16(src.type)) file->addCode8(0x66);
                writeRexPrefix(src.type, EmptyToken);
                
                // Encode the opcode
                if (isRegister8(src.type)) file->addCode8(0x88);
                else file->addCode8(0x89);
                
                // The operand
                writeDspOperand(1, base.type, src.type, offset);
            }
        }
        
    // Build register destinations
    } else {
        scanner->getNext();
        Token src = scanner->getNext();
        
        switch (src.type) {
            // Immediate-to-register
            //
            // This one is a pain because it varies for each register type
            //
            case Int32: {
                if (op == Mov) {
                    writeMovI(op, dest.type, src.i32_val);
                } else {
                    // Write the prefixes
                    if (isRegister16(dest.type)) file->addCode8(0x66);
                    writeRexPrefix(EmptyToken, dest.type);
                    
                    // Write the rest of the instruction
                    writeAluI(op, dest.type);
                    file->addCode8(src.i32_val);
                }
            } break;
            
            // Local label
            //
            // Same as immediate-to-register, except with zero
            case OFFSET: {
                scanner->getNext();         // FLAT
                scanner->getNext();         // COLON
                scanner->getNext();         // ID
                
                writeMovI(op, dest.type, 0);
            } break;
            
            case LBrace: {
                Token base = scanner->getNext();
                int offset = 1;
                
                Token next = scanner->getNext();
                if (next.type == RBrace) {
                    // TODO... a whole other problem
                }
                
                Token offsetToken = scanner->getNext();
                if (isRegister(offsetToken.type)) {
                    // TODO
                } else if (offsetToken.type == Int32) {
                    offset = offsetToken.i32_val;
                    if (next.type == Minus) offset *= -1;
                    
                    scanner->getNext();                 // Consume ']'
                    
                    // Now, encode
                    // Start with any prefixes
                    if (isRegister16(dest.type)) file->addCode8(0x66);
                    writeRexPrefix(dest.type, EmptyToken);
                    
                    // Opcode
                    if (op == Mov) {
                        if (isRegister8(dest.type)) file->addCode8(0x8A);
                        else file->addCode8(0x8B);
                    } else if (op == Lea) {
                        file->addCode8(0x8D);
                    }
                    
                    // Operand
                    writeDspOperand(1, base.type, dest.type, offset);
                }
            } break;
            
            // TODO: Merge with above
            case DWORD: {
                Token token = scanner->getNext();       // Consume PTR
                token = scanner->getNext();             // Consume '['
                Token base = scanner->getNext();     // Get the register
                token = scanner->getNext();             // Consume the operator
                Token offset = scanner->getNext();      // Get the integer
                token = scanner->getNext();             // Get the ']'
                
                // Prefixes
                if (isRegister16(src.type)) file->addCode8(0x66);
                writeRexPrefix(src.type, EmptyToken);
                
                // Encode the opcode
                if (op == Mov) {
                    //if (isRegister8(src.type)) file->addCode8(0x88);
                    //else file->addCode8(0x89);
                    file->addCode8(0x8B);
                } else if (op == Lea) {
                    file->addCode8(0x8D);
                }
                
                // The operand
                writeDspOperand(1, base.type, dest.type, offset.i32_val);
            } break;
            
            default: {
                if (!isRegister(src.type)) break;
                
                if (isRegister16(dest.type)) file->addCode8(0x66);
                
                // Default to registers
                writeRexPrefix(src.type, dest.type);
                if (isRegister8(dest.type)) {
                    if (op == Add) file->addCode8(0x00);
                    else if (op == Sub) file->addCode8(0x28);
                    else if (op == And) file->addCode8(0x20);
                    else if (op == Or) file->addCode8(0x08);
                    else if (op == Xor) file->addCode8(0x30);
                    else if (op == Mov) file->addCode8(0x88);
                } else {
                    if (op == Add) file->addCode8(0x01);
                    else if (op == Sub) file->addCode8(0x29);
                    else if (op == And) file->addCode8(0x21);
                    else if (op == Or) file->addCode8(0x09);
                    else if (op == Xor) file->addCode8(0x31);
                    else if (op == Mov) file->addCode8(0x89);
                }
                
                writeRROperand(3, src.type, dest.type);
            }
        }
    }
}

// Mov-immediates are something else on Intel
//
void Parser::writeMovI(TokenType op, TokenType dest, int value) {
    // 8-bit mov-i
    if (isRegister8(dest)) {
        if (dest == Sil || dest == Dil || dest == Spl || dest == Bpl) {
            file->addCode8(0x40);
        } else if (isRegisterExt(dest)) {
            file->addCode8(0x41);
        }
    
        switch (dest) {
            case Al: case R8b: file->addCode8(0xB0); break;
            case Cl: case R9b: file->addCode8(0xB1); break;
            case Dl: case R10b: file->addCode8(0xB2); break;
            case Bl: case R11b: file->addCode8(0xB3); break;
            case Spl: case Ah: case R12b: file->addCode8(0xB4); break;
            case Bpl: case Ch: case R13b: file->addCode8(0xB5); break;
            case Sil: case Dh: case R14b: file->addCode8(0xB6); break;
            case Dil: case Bh: case R15b: file->addCode8(0xB7); break;
            
            default: {}
        }
    
    // 64-bit mov-i
    } else if (isRegister64(dest)) {
        writeRexPrefix(EmptyToken, dest);
        file->addCode8(0xC7);

        switch (dest) {
            case Rax: case R8: file->addCode8(0xC0); break;
            case Rcx: case R9: file->addCode8(0xC1); break;
            case Rdx: case R10: file->addCode8(0xC2); break;
            case Rbx: case R11: file->addCode8(0xC3); break;
            case Rsp: case R12: file->addCode8(0xC4); break;
            case Rbp: case R13: file->addCode8(0xC5); break;
            case Rsi: case R14: file->addCode8(0xC6); break;
            case Rdi: case R15: file->addCode8(0xC7); break;
            
            default: {}
        }
        
    // 16/32-bit mov-i
    } else {
        if (isRegister16(dest)) {
            file->addCode8(0x66);
        }
        writeRexPrefix(EmptyToken, dest);

        switch (dest) {
            case Ax: case R8w:
            case Eax: case R8d: file->addCode8(0xB8); break;
            
            case Cx: case R9w:
            case Ecx: case R9d: file->addCode8(0xB9); break;
            
            case Dx: case R10w:
            case Edx: case R10d: file->addCode8(0xBA); break;
            
            case Bx: case R11w:
            case Ebx: case R11d: file->addCode8(0xBB); break;
            
            case Sp: case R12w:
            case Esp: case R12d: file->addCode8(0xBC); break;
            
            case Bp: case R13w:
            case Ebp: case R13d: file->addCode8(0xBD); break;
            
            case Si: case R14w:
            case Esi: case R14d: file->addCode8(0xBE); break;
            
            case Di: case R15w:
            case Edi: case R15d: file->addCode8(0xBF); break;
            
            default: {}
        }
    }

    if (isRegister8(dest)) file->addCode8(value);
    else if (isRegister16(dest)) file->addCode16(value);
    else file->addCode32(value);
}

