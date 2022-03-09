#pragma once

#include <string>
#include <map>

#include "lex.hpp"

struct Symbols {
    std::map<std::string, int> locations;
    std::map<std::string, bool> global;
};

class SymbolParser {
public:
    explicit SymbolParser(std::string input);
    Symbols *getSymbols();
private:
    // Private variables
    Scanner *scanner;
    Symbols *symbols;
    int location = 0;
    
    // Private functions
    void parseText();
    void parseMov();
    int getRegSize(TokenType type);
};

