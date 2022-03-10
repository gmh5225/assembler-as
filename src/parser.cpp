#include "parser.hpp"

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
            case Mov: buildStdInstr(token.type); break;
            
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
    if (dest.type == DWORD) {
        scanner->getNext();     // consume PTR
        scanner->getNext();     // consume '['
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
            file->addCode8(0xC7);
            writeDspOperand(1, base.type, EmptyToken, offset);
            file->addCode32(src.i32_val);
        }
        
    // Build register destinations
    } else {
        scanner->getNext();
        Token src = scanner->getNext();
        
        // Register-to-register instruction -> 32 bits
        switch (src.type) {
            case Int32: {
                writeRexPrefix(EmptyToken, dest.type);
                
                switch (dest.type) {
                    case Eax: case R8d: file->addCode8(0xB8); break;
                    case Ecx: case R9d: file->addCode8(0xB9); break;
                    case Edx: case R10d: file->addCode8(0xBA); break;
                    case Ebx: case R11d: file->addCode8(0xBB); break;
                    case Esp: case R12d: file->addCode8(0xBC); break;
                    case Ebp: case R13d: file->addCode8(0xBD); break;
                    case Esi: case R14d: file->addCode8(0xBE); break;
                    case Edi: case R15d: file->addCode8(0xBF); break;
                    
                    default: {}
                }
                
                file->addCode32(src.i32_val);
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
                    file->addCode8(0x8B);
                    writeDspOperand(1, base.type, dest.type, offset);
                }
            } break;
            
            default: {
                if (!isRegister(src.type)) break;
                
                // Default to registers
                writeRexPrefix(src.type, dest.type);
                if (op == Add) file->addCode8(0x01);
                else if (op == Sub) file->addCode8(0x29);
                else if (op == And) file->addCode8(0x21);
                else if (op == Or) file->addCode8(0x09);
                else if (op == Xor) file->addCode8(0x31);
                else if (op == Mov) file->addCode8(0x89);
                
                writeRROperand(3, src.type, dest.type);
            }
        }
    }
}

// Encodes a REX prefix if needed (see section 2.2.1)
//
// Encoding: 4 <64 bit- 0,1> <src extended- 0,1 > 0 <dest extend- 0,1>
//
void Parser::writeRexPrefix(TokenType src, TokenType dest) {
    // First, check if prefix is even needed
    // Its not needed IF the registers are not 64-bit and they aren't extended
    if ((!isRegister64(src) && !isRegister64(dest)) && (!isRegisterExt(src) && !isRegisterExt(dest))) {
        return;
    }
    
    uint8_t output = 0x40;
    
    if (isRegister64(src) || isRegister64(dest)) {
        output &= 0b11110101;
        output |= 0b00001000;
    }
    
    if (isRegisterExt(src)) {
        output &= 0b11111001;
        output |= 0b00000100;
    }
    
    if (isRegisterExt(dest)) {
        output &= 0b11111100;
        output |= 0b00000001;
    }
    
    file->addCode8(output);
}

// Builds a register-register operand
//
// Encoding: <prefix: 0-3> <src> <dest>
//
void Parser::writeRROperand(uint8_t prefix, TokenType src, TokenType dest) {
    uint8_t srcReg = getRegisterValue(src);
    uint8_t destReg = getRegisterValue(dest);

    uint8_t output = 0;
    
    output &= 0b00111111;
    output |= (prefix << 6);
    
    output &= 0b11000111;
    output |= (srcReg << 3);
    
    output &= 0b11111000;
    output |= destReg;
    
    file->addCode8(output);
}

// Builds a memory-displacement operand
//
// Encoding: <size prefix: 0-3> <offset reg> <base reg> | <offset>
void Parser::writeDspOperand(uint8_t size, TokenType base, TokenType regOffset, int offset) {
    uint8_t offsetReg = getRegisterValue(regOffset);
    uint8_t baseReg = getRegisterValue(base);
    
    uint8_t output = 0;
    
    output &= 0b00111111;
    output |= (size << 6);
    
    output &= 0b11000111;
    output |= (offsetReg << 3);
    
    output &= 0b11111000;
    output |= baseReg;
    
    file->addCode8(output);
    file->addCode8((int8_t)offset);
}

uint8_t Parser::getRegisterValue(TokenType reg) {
    switch (reg) {
        case Eax: case Rax:
        case R8d: return 0;
        
        case Ecx: case Rcx:
        case R9d: return 1;
        
        case Edx: case Rdx:
        case R10d: return 2;
        
        case Ebx: case Rbx:
        case R11d: return 3;
        
        case Esp: case Rsp:
        case R12d: return 4;
        
        case Ebp: case Rbp:
        case R13d: return 5;
        
        case Esi: case Rsi:
        case R14d: return 6;
        
        case Edi: case Rdi: 
        case R15d: return 7;
        
        default: {}
    }
    
    return 0;
}

bool Parser::isRegister(TokenType reg) {
    switch (reg) {
        case Eax:
        case Ecx:
        case Edx:
        case Ebx:
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
        case R15d: return true;
        
        case Rax:
        case Rcx:
        case Rdx:
        case Rbx:
        case Rsp:
        case Rbp:
        case Rsi:
        case Rdi: return true;
        
        default: {}
    }
    
    return false;
}

bool Parser::isRegister64(TokenType reg) {
    switch (reg) {
        case Rax:
        case Rcx:
        case Rdx:
        case Rbx:
        case Rsp:
        case Rbp:
        case Rsi:
        case Rdi: return true;
        
        default: {}
    }
    return false;
}

bool Parser::isRegisterExt(TokenType reg) {
    switch (reg) {
        case R8d:
        case R9d:
        case R10d:
        case R11d:
        case R12d:
        case R13d:
        case R14d:
        case R15d: return true;
        
        default: {}
    }
    return false;
}

