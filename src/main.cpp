#include <iostream>

#include "elf.hpp"
#include "symbol.hpp"

int main(int argc, char *argv[]) {
    Elf64File file("first.bin");
    file.addFunctionSymbol("_start", 0, true);
    
    // Write some code:
    //-- mov eax, 60
    //-- mov edi, 6
    //-- syscall
    file.addCode8(0xB8);
    file.addCode32(60);
    
    file.addCode8(0xBF);
    file.addCode32(5);
    
    file.addCode8(0x0F);
    file.addCode8(0x05);
    
    file.write();
    
    // Test out our symbol parser
    SymbolParser symparse("./first.asm");
    Symbols *symbols = symparse.getSymbols();
    
    for (auto const &x : symbols->locations) {
        std::cout << x.first << " -> " << x.second << std::endl;
    }
    
    return 0;
}

