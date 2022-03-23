#pragma once

#include <string>

#include "lex.hpp"
#include <libelf/elf.hpp>
#include "symbol.hpp"

class Parser {
public:
    explicit Parser(std::string input, Elf64File *file, Symbols *symbols);
    void build();
private:
    // Private variables
    Scanner *scanner;
    Elf64File *file;
    Symbols *symbols;
    
    // Private functions
    void parseText();
    void buildPush();
    void buildStdInstr(TokenType op);
    void writeMovI(TokenType op, TokenType dest, int value);
    
    // In encoding.cpp
    void writeAluI(TokenType op, TokenType dest);
    void writeRexPrefix(TokenType src, TokenType dest);
    void writeRROperand(uint8_t prefix, TokenType src, TokenType dest);
    void writeDspOperand(uint8_t size, TokenType base, TokenType regOffset, int offset);
    uint8_t getRegisterValue(TokenType reg);
};

