#pragma once

#include <string>
#include <map>
#include <vector>

#include "lex.hpp"
#include "elf.hpp"

struct Symbols {
    std::map<std::string, int> locations;
    std::vector<std::string> global;
    std::map<int, int> jumps;
    
    // .data values
    std::vector<std::string> data_strings;
    std::map<std::string, int> data_locations;
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
    int dataLocation = 0;
    
    // Private functions
    void parseData();
    void parseText();
    void parseStdInstr(TokenType op);
    int getRegSize(TokenType type);
};

