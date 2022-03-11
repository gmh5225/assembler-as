#include <iostream>
#include <cctype>

#include "lex.hpp"

// The token debug function
Token::Token() {
    type = EmptyToken;
    id_val = "";
    i32_val = 0;
}

// The scanner functions
Scanner::Scanner(std::string input) {
    reader = std::ifstream(input.c_str());
    if (!reader.is_open()) {
        std::cout << "Unknown input file." << std::endl;
        error = true;
    }
}

Scanner::~Scanner() {
    reader.close();
}

void Scanner::rewind(Token token) {
    token_stack.push(token);
}

// The main scanning function
Token Scanner::getNext() {
    if (token_stack.size() > 0) {
        Token top = token_stack.top();
        token_stack.pop();
        return top;
    }

    Token token;
    if (reader.eof()) {
        token.type = Eof;
        return token;
    }
    
    for (;;) {
        char next = reader.get();
        if (reader.eof()) {
            token.type = Eof;
            break;
        }
        
        rawBuffer += next;
        
        
        
        
        // TODO: This needs some kind of error handleing
        if (next == '\'') {
            char c = reader.get();
            rawBuffer += c;
            if (c == '\\') {
                c = reader.get();
                if (c == 'n') {
                    c = '\n';
                    rawBuffer += c;
                }
            }
        
            Token charL;
            charL.i8_val = c;
            charL.type = CharL;
            
            next = reader.get();
            rawBuffer += next;
            return charL;
        }
        
        if (next == '\"') {
            if (inQuote) {
                Token str;
                str.type = String;
                str.id_val = buffer;
                
                buffer = "";
                inQuote = false;
                return str;
            } else {
                inQuote = true;
                continue;
            }
        }
        
        if (inQuote) {
            if (next == '\\') {
                next = reader.get();
                rawBuffer += next;
                switch (next) {
                    case 'n': buffer += '\n'; break;
                    case 't': buffer += '\t'; break;
                    default: buffer += '\\' + next;
                }
            } else {
                buffer += next;
            }
            continue;
        }
        
        if (next == ' ' || next == '\n' || isSymbol(next)) {
            if (next == '\n') {
                if (skipNextLineCount) skipNextLineCount = false;
                else ++currentLine;
            }
        
            if (buffer.length() == 0) {
                if (isSymbol(next)) {
                    Token sym;
                    sym.type = getSymbol(next);
                    return sym;
                }
                continue;
            }
            
            // Check if we have a symbol
            // Here, we also check to see if we have a floating point
            if (next == '.') {
                if (isInt()) {
                    buffer += ".";
                    continue;
                } else {
                    Token sym;
                    sym.type = getSymbol(next);
                    token_stack.push(sym);
                }
            } else if (isSymbol(next)) {
                Token sym;
                sym.type = getSymbol(next);
                token_stack.push(sym);
            }
            
            // Now check the buffer
            token.type = getKeyword();
            if (token.type != EmptyToken) {
                buffer = "";
                break;
            }
            
            if (isInt()) {
                token.type = Int32;
                token.i32_val = std::stoi(buffer);
            } else if (isHex()) {
                token.type = Int32;
                token.i32_val = std::stoi(buffer, 0, 16);
            } else {
                token.type = Id;
                token.id_val = buffer;
            }
            
            // Reset everything
            buffer = "";
            break;
        } else {
            buffer += next;
        }
    }
    
    return token;
}

std::string Scanner::getRawBuffer() {
    std::string ret = rawBuffer;
    rawBuffer = "";
    return ret;
}

bool Scanner::isSymbol(char c) {
    switch (c) {
        //case ';':
        case ':': return true;
        case ',': return true;
        case '[': return true;
        case ']': return true;
        case '-': return true;
        case '+': return true;
        
        default: return false;
    }
    return false;
}

TokenType Scanner::getKeyword() {
    //if (buffer == "extern") return Extern;
    if (buffer == "mov") return Mov;
    else if (buffer == "syscall") return Syscall;
    else if (buffer == "ret") return Ret;
    else if (buffer == "push") return Push;
    else if (buffer == "add") return Add;
    else if (buffer == "sub") return Sub;
    else if (buffer == "and") return And;
    else if (buffer == "or") return Or;
    else if (buffer == "xor") return Xor;
    else if (buffer == "ax") return Ax;
    else if (buffer == "bx") return Bx;
    else if (buffer == "cx") return Cx;
    else if (buffer == "dx") return Dx;
    else if (buffer == "sp") return Sp;
    else if (buffer == "bp") return Bp;
    else if (buffer == "si") return Si;
    else if (buffer == "di") return Di;
    else if (buffer == "r8w") return R8w;
    else if (buffer == "r9w") return R9w;
    else if (buffer == "r10w") return R10w;
    else if (buffer == "r11w") return R11w;
    else if (buffer == "r12w") return R12w;
    else if (buffer == "r13w") return R13w;
    else if (buffer == "r14w") return R14w;
    else if (buffer == "r15w") return R15w;
    else if (buffer == "eax") return Eax;
    else if (buffer == "ebx") return Ebx;
    else if (buffer == "ecx") return Ecx;
    else if (buffer == "edx") return Edx;
    else if (buffer == "esp") return Esp;
    else if (buffer == "ebp") return Ebp;
    else if (buffer == "esi") return Esi;
    else if (buffer == "edi") return Edi;
    else if (buffer == "r8d") return R8d;
    else if (buffer == "r9d") return R9d;
    else if (buffer == "r10d") return R10d;
    else if (buffer == "r11d") return R11d;
    else if (buffer == "r12d") return R12d;
    else if (buffer == "r13d") return R13d;
    else if (buffer == "r14d") return R14d;
    else if (buffer == "r15d") return R15d;
    else if (buffer == "rax") return Rax;
    else if (buffer == "rbx") return Rbx;
    else if (buffer == "rcx") return Rcx;
    else if (buffer == "rdx") return Rdx;
    else if (buffer == "rsp") return Rsp;
    else if (buffer == "rbp") return Rbp;
    else if (buffer == "rsi") return Rsi;
    else if (buffer == "rdi") return Rdi;
    else if (buffer == "r8") return R8;
    else if (buffer == "r9") return R9;
    else if (buffer == "r10") return R10;
    else if (buffer == "r11") return R11;
    else if (buffer == "r12") return R12;
    else if (buffer == "r13") return R13;
    else if (buffer == "r14") return R14;
    else if (buffer == "r15") return R15;
    else if (buffer == "DWORD") return DWORD;
    else if (buffer == "PTR") return PTR;
    return EmptyToken;
}

TokenType Scanner::getSymbol(char c) {
    switch (c) {
        case ':': return Colon;
        case ',': return Comma;
        case '[': return LBrace;
        case ']': return RBrace;
        case '-': return Minus;
        case '+': return Plus;
        default: return EmptyToken;
    }
    return EmptyToken;
}

bool Scanner::isInt() {
    for (char c : buffer) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool Scanner::isHex() {
    if (buffer.length() < 3) return false;
    if (buffer[0] != '0' || buffer[1] != 'x') return false;
    
    for (int i = 2; i<buffer.length(); i++) {
        if (!isxdigit(buffer[i])) return false;
    }
    return true;
}

