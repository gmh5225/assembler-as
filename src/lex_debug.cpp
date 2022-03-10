#include <iostream>

#include "lex.hpp"

void Token::print() {
    switch (this->type) {
        case EmptyToken: std::cout << "???" << std::endl; break;
        case Eof: std::cout << "EOF" << std::endl; break;
        
        case Mov: std::cout << "mov" << std::endl; break;
        case Syscall: std::cout << "syscall" << std::endl; break;
        case Ret: std::cout << "ret" << std::endl; break;
        case Push: std::cout << "push" << std::endl; break;
        case Add: std::cout << "add" << std::endl; break;
        case Sub: std::cout << "sub" << std::endl; break;
        case And: std::cout << "and" << std::endl; break;
        case Or: std::cout << "or" << std::endl; break;
        case Xor: std::cout << "xor" << std::endl; break;
        case Eax: std::cout << "eax" << std::endl; break;
        case Ebx: std::cout << "ebx" << std::endl; break;
        case Ecx: std::cout << "ecx" << std::endl; break;
        case Edx: std::cout << "edx" << std::endl; break;
        case Esp: std::cout << "esp" << std::endl; break;
        case Ebp: std::cout << "ebp" << std::endl; break;
        case Esi: std::cout << "esi" << std::endl; break;
        case Edi: std::cout << "edi" << std::endl; break;
        case Rax: std::cout << "rax" << std::endl; break;
        case Rbx: std::cout << "rbx" << std::endl; break;
        case Rcx: std::cout << "rcx" << std::endl; break;
        case Rdx: std::cout << "rdx" << std::endl; break;
        case Rsp: std::cout << "rsp" << std::endl; break;
        case Rbp: std::cout << "rbp" << std::endl; break;
        case Rsi: std::cout << "rsi" << std::endl; break;
        case Rdi: std::cout << "rdi" << std::endl; break;
        case DWORD: std::cout << "DWORD" << std::endl; break;
        case PTR: std::cout << "PTR" << std::endl; break;
        case Colon: std::cout << ":" << std::endl; break;
        case Comma: std::cout << "," << std::endl; break;
        case LBrace: std::cout << "[" << std::endl; break;
        case RBrace: std::cout << "]" << std::endl; break;
        case Minus: std::cout << "-" << std::endl; break;
        case Plus: std::cout << "+" << std::endl; break;
        
        case Id: std::cout << "ID(" << id_val << ")" << std::endl; break;
        case String: std::cout << "STR(" << id_val << ")" << std::endl; break;
        case CharL: std::cout << "CHAR(" << i8_val << ")" << std::endl; break;
        case Int32: std::cout << "INT(" << i32_val << ")" << std::endl; break;
    }
}

