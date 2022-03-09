#include <iostream>

#include "lex.hpp"

void Token::print() {
    switch (this->type) {
        case EmptyToken: std::cout << "???" << std::endl; break;
        case Eof: std::cout << "EOF" << std::endl; break;
        
        case Mov: std::cout << "mov" << std::endl; break;
        case Syscall: std::cout << "syscall" << std::endl; break;
        case Ret: std::cout << "ret" << std::endl; break;
        case Eax: std::cout << "eax" << std::endl; break;
        case Ebx: std::cout << "ebx" << std::endl; break;
        case Ecx: std::cout << "ecx" << std::endl; break;
        case Edx: std::cout << "edx" << std::endl; break;
        case Esi: std::cout << "esi" << std::endl; break;
        case Edi: std::cout << "edi" << std::endl; break;
        case Colon: std::cout << ":" << std::endl; break;
        case Comma: std::cout << "," << std::endl; break;
        
        case Id: std::cout << "ID(" << id_val << ")" << std::endl; break;
        case String: std::cout << "STR(" << id_val << ")" << std::endl; break;
        case CharL: std::cout << "CHAR(" << i8_val << ")" << std::endl; break;
        case Int32: std::cout << "INT(" << i32_val << ")" << std::endl; break;
    }
}

