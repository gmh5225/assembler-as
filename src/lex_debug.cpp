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
        case Ax: std::cout << "ax" << std::endl; break;
        case Bx: std::cout << "bx" << std::endl; break;
        case Cx: std::cout << "cx" << std::endl; break;
        case Dx: std::cout << "dx" << std::endl; break;
        case Sp: std::cout << "sp" << std::endl; break;
        case Bp: std::cout << "bp" << std::endl; break;
        case Si: std::cout << "si" << std::endl; break;
        case Di: std::cout << "di" << std::endl; break;
        case R8w: std::cout << "r8w" << std::endl; break;
        case R9w: std::cout << "r9w" << std::endl; break;
        case R10w: std::cout << "r10w" << std::endl; break;
        case R11w: std::cout << "r11w" << std::endl; break;
        case R12w: std::cout << "r12w" << std::endl; break;
        case R13w: std::cout << "r13w" << std::endl; break;
        case R14w: std::cout << "r14w" << std::endl; break;
        case R15w: std::cout << "r15w" << std::endl; break;
        case Eax: std::cout << "eax" << std::endl; break;
        case Ebx: std::cout << "ebx" << std::endl; break;
        case Ecx: std::cout << "ecx" << std::endl; break;
        case Edx: std::cout << "edx" << std::endl; break;
        case Esp: std::cout << "esp" << std::endl; break;
        case Ebp: std::cout << "ebp" << std::endl; break;
        case Esi: std::cout << "esi" << std::endl; break;
        case Edi: std::cout << "edi" << std::endl; break;
        case R8d: std::cout << "r8d" << std::endl; break;
        case R9d: std::cout << "r9d" << std::endl; break;
        case R10d: std::cout << "r10d" << std::endl; break;
        case R11d: std::cout << "r11d" << std::endl; break;
        case R12d: std::cout << "r12d" << std::endl; break;
        case R13d: std::cout << "r13d" << std::endl; break;
        case R14d: std::cout << "r14d" << std::endl; break;
        case R15d: std::cout << "r15d" << std::endl; break;
        case Rax: std::cout << "rax" << std::endl; break;
        case Rbx: std::cout << "rbx" << std::endl; break;
        case Rcx: std::cout << "rcx" << std::endl; break;
        case Rdx: std::cout << "rdx" << std::endl; break;
        case Rsp: std::cout << "rsp" << std::endl; break;
        case Rbp: std::cout << "rbp" << std::endl; break;
        case Rsi: std::cout << "rsi" << std::endl; break;
        case Rdi: std::cout << "rdi" << std::endl; break;
        case R8: std::cout << "r8" << std::endl; break;
        case R9: std::cout << "r9" << std::endl; break;
        case R10: std::cout << "r10" << std::endl; break;
        case R11: std::cout << "r11" << std::endl; break;
        case R12: std::cout << "r12" << std::endl; break;
        case R13: std::cout << "r13" << std::endl; break;
        case R14: std::cout << "r14" << std::endl; break;
        case R15: std::cout << "r15" << std::endl; break;
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

