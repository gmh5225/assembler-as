#include "parser.hpp"
#include "lex.hpp"
#include "register.hpp"

// Encodes an ALU-I instruction
//
// Encoding: 0x83 | 11 <ALU OP> <dest reg>
// OPS
// Add: 000
// Sub:
// And:
// Or:
// Xor:
//
void Parser::writeAluI(TokenType op, TokenType dest) {
    uint8_t output = 0xC0;
    uint8_t destReg = getRegisterValue(dest);
    uint8_t alu_op = 0;         // Default to add == 0
    
    switch (op) {
        case Sub: alu_op = 5; break;
        case And: alu_op = 4; break;
        case Or: alu_op = 1; break;
        case Xor: alu_op = 6; break;
        default: {}
    }
    
    // Encode the ALU operation
    output &= 0b11000111;
    output |= (alu_op << 3);
    
    // Encode the register
    output &= 0b11111000;
    output |= destReg;
    
    if (isRegister8(dest)) file->addCode8(0x80);
    else file->addCode8(0x83);
    file->addCode8(output);
}

// Encodes a REX prefix if needed (see section 2.2.1)
//
// Encoding: 4 <64 bit- 0,1> <src extended- 0,1 > 0 <dest extend- 0,1>
//
// Note when we are using 8-bit registers:
// If we are using SPL, BPL, SIL, DIL-> we just need the REX prefix.
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
    
    /**if (src == Spl || src == Bpl || src == Sil || src == Dil) {
        if (dest == Spl || dest == Bpl || dest == Sil || dest == Dil) {
        puts("IN");
            output = 0x45;
            file->addCode8(output);
            return;
        }
    }*/
    
    if (isRegisterExt(src)) {
        output &= 0b11111001;
        
        switch (src) {
            case Spl:
            case Bpl:
            case Sil:
            case Dil: output |= 0b00000000; break;
            
            default: output |= 0b00000100;
        }
    }
    
    if (isRegisterExt(dest)) {
        output &= 0b11111100;
        
        switch (dest) {
            case Spl:
            case Bpl:
            case Sil:
            case Dil: output |= 0b00000000; break;
            
            default: output |= 0b00000001;
        }
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
        case Al: case R8b:
        case Ax: case R8w:
        case Eax: case Rax:
        case R8d: case R8: return 0;
        
        case Cl: case R9b:
        case Cx: case R9w:
        case Ecx: case Rcx:
        case R9d: case R9: return 1;
        
        case Dl: case R10b:
        case Dx: case R10w:
        case Edx: case Rdx:
        case R10d: case R10: return 2;
        
        case Bl: case R11b:
        case Bx: case R11w:
        case Ebx: case Rbx:
        case R11d: case R11: return 3;
        
        case Ah: case Spl: case R12b:
        case Sp: case R12w:
        case Esp: case Rsp:
        case R12d: case R12: return 4;
        
        case Ch: case Bpl: case R13b:
        case Bp: case R13w:
        case Ebp: case Rbp:
        case R13d: case R13: return 5;
        
        case Dh: case Sil: case R14b:
        case Si: case R14w:
        case Esi: case Rsi:
        case R14d: case R14: return 6;
        
        case Bh: case Dil: case R15b:
        case Di: case R15w:
        case Edi: case Rdi: 
        case R15d: case R15: return 7;
        
        default: {}
    }
    
    return 0;
}

