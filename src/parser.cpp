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
            case Mov: buildMov(); break;
            
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

void Parser::buildMov() {
    Token dest = scanner->getNext();
    scanner->getNext();
    Token src = scanner->getNext();
    
    switch (src.type) {
        case Eax:
        case Ecx:
        case Edx:
        case Ebx:
        case Esp:
        case Ebp:
        case Esi:
        case Edi: {
            file->addCode8(0x89);
            writeRROperand(3, src.type, dest.type);
        } break;
        
        case Int32: {
            switch (dest.type) {
                case Eax: file->addCode8(0xB8); break;
                case Ecx: file->addCode8(0xB9); break;
                case Edx: file->addCode8(0xBA); break;
                case Ebx: file->addCode8(0xBB); break;
                case Esp: file->addCode8(0xBC); break;
                case Ebp: file->addCode8(0xBD); break;
                case Esi: file->addCode8(0xBE); break;
                case Edi: file->addCode8(0xBF); break;
                
                default: {}
            }
            
            file->addCode32(src.i32_val);
        } break;
        
        // TODO: Others
    }
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

uint8_t Parser::getRegisterValue(TokenType reg) {
    switch (reg) {
        case Eax: return 0;
        case Ecx: return 1;
        case Edx: return 2;
        case Ebx: return 3;
        case Esp: return 4;
        case Ebp: return 5;
        case Esi: return 6;
        case Edi: return 7;
        
        default: {}
    }
    
    return 0;
}

