#pragma once

#include <string>
#include <map>
#include <vector>

#include "lex.hpp"
#include "elf.hpp"

struct Symbols {
    std::map<std::string, int> locations;
    std::vector<std::string> global;
};

class SymbolParser {
public:
    explicit SymbolParser(std::string input);
    Symbols *getSymbols();
    void processSymbols(Elf64File *file);
private:
    // Private variables
    Scanner *scanner;
    Symbols *symbols;
    int location = 0;
    
    // Private functions
    void parseText();
    void parseStdInstr();
    int getRegSize(TokenType type);
};

