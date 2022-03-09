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
    Token src = scanner->getNext();
    scanner->getNext();
    Token dest = scanner->getNext();
    
    switch (dest.type) {
        case Int32: {
            switch (src.type) {
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
            
            file->addCode32(dest.i32_val);
        } break;
        
        // TODO: Others
    }
}

