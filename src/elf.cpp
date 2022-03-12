#include <stdio.h>

#include "elf.hpp"

Elf64File::Elf64File(std::string name) {
    this->name = name;
    
    // Init the header
    header = new Elf64_Header;
    header->e_ident[0] = 0x7F;
    header->e_ident[1] = 0x45;
    header->e_ident[2] = 0x4C;
    header->e_ident[3] = 0x46;
    header->e_ident[4] = 2;
    header->e_ident[5] = 1;
    header->e_ident[6] = 1;
    //header->e_ident[7] = 3;
    //header->e_ident[8] = 1;
    for (int i = 7; i<EI_NIDENT; i++) header->e_ident[i] = 0;
    
    header->e_type = 1;
    header->e_machine = 62;
    header->e_version = 1;
    header->e_entry = 0;
    header->e_phoff = 0;
    header->e_shoff = 64;       // Location of start of section headers (always 64)
    header->e_flags = 0;
    header->e_ehsize = 64;
    header->e_phentsize = 0;
    header->e_phnum = 0;
    header->e_shentsize = 64;       // Size of section headers (always 64)
    header->e_shnum = 1;            // Number of section headers
    header->e_shstrndx = 0;         // Section header string table index
    
    // Create the null header
    Elf64_Shdr *nullHdr = new Elf64_Shdr;
    sections.push_back(nullHdr);
    
    //
    // Section string table
    //
    shstrtable = new ElfSectionStrTable;
    shstrtable->header = new Elf64_Shdr;
    shstrtable->header->sh_name = 1;
    shstrtable->header->sh_type = 3;        // SHT_STRTAB
    shstrtable->header->sh_flags = 0;
    shstrtable->header->sh_addr = 0;
    shstrtable->header->sh_link = 0;
    shstrtable->header->sh_info = 0;
    shstrtable->header->sh_addralign = 1;
    shstrtable->header->sh_entsize = 0;
    sections.push_back(shstrtable->header);
    
    shstrtable->table.push_back(".shstrtab");
    
    //
    // Regular string table
    //
    shstrtable->table.push_back(".strtab");
    
    strtab = new ElfStrTable;
    strtab->header = new Elf64_Shdr;
    strtab->header->sh_name = getSectionNamePos(".strtab");
    strtab->header->sh_type = 3;        // SHT_STRTAB
    strtab->header->sh_addralign = 1;
    
    sections.push_back(strtab->header);
    strtab->index = sections.size() - 1;
    
    //
    // Section .data
    //
    shstrtable->table.push_back(".data");
    
    data = new ElfData;
    data->header = new Elf64_Shdr;
    data->header->sh_name = getSectionNamePos(".data");
    data->header->sh_type = 1;          // SHT_PROGBITS
    data->header->sh_flags = 3;         // SHF_WRITE | SHF_ALLOC
    //data->header->sh_addralign = 4;
    data->header->sh_addralign = 1;
    
    sections.push_back(data->header);
    data->index = sections.size() - 1;
    
    //
    // Section .text
    //
    shstrtable->table.push_back(".text");
    
    text = new ElfText;
    text->header = new Elf64_Shdr;
    text->header->sh_name = getSectionNamePos(".text");
    text->header->sh_type = 1;          // SHT_PROGBITS
    text->header->sh_flags = 6;         // SHF_WRITE | SHF_ALLOC
    text->header->sh_addralign = 1;
    
    sections.push_back(text->header);
    text->index = sections.size() - 1;
    
    //
    // Symbol table
    //
    shstrtable->table.push_back(".symtab");
    
    symtab = new ElfSymTable;
    symtab->header = new Elf64_Shdr;
    symtab->header->sh_name = getSectionNamePos(".symtab");
    symtab->header->sh_type = 2;                                // SHT_SYMTAB
    symtab->header->sh_flags = 0;
    symtab->header->sh_addr = 0;
    symtab->header->sh_link = strtab->index;        // Connects to .strtab
    symtab->header->sh_info = 0;                    // Elf64_Word(start_pos)
    //symtab->header->sh_addralign = text->index;     // Connects to .text
    symtab->header->sh_addralign = 8;
    symtab->header->sh_entsize = sizeof(Elf64_Sym);
    
    sections.push_back(symtab->header);
    symtab->index = sections.size() - 1;
    
    //
    // Section .rela.text
    //
    shstrtable->table.push_back(".rela.text");
    
    rela_text = new ElfRelaText;
    rela_text->header = new Elf64_Shdr;
    rela_text->header->sh_name = getSectionNamePos(".rela.text");
    rela_text->header->sh_type = 4;                                     // SHT_RELA
    rela_text->header->sh_link = symtab->index;
    rela_text->header->sh_info = text->index;
    rela_text->header->sh_addralign = 8;
    rela_text->header->sh_entsize = sizeof(Elf64_Rela);
    
    sections.push_back(rela_text->header);
    rela_text->index = sections.size() - 1;
    
    //
    // Load the default symbol table
    //
    Elf64_Sym *nullSym = new Elf64_Sym;
    symtab->symbols.push_back(nullSym);
    
    strtab->table.push_back(name);
    Elf64_Sym *fileSym = new Elf64_Sym;
    fileSym->st_name = getStringPos(name);
    fileSym->st_info = 4;                       // STB_LOCAL | STT_FILE
    fileSym->st_other = 0;                      // STV_DEFAULT
    fileSym->st_shndx = SHN_ABS;
    symtab->symbols.push_back(fileSym);
    
    Elf64_Sym *dataSym = new Elf64_Sym;
    dataSym->st_info = 3;                   // STB_LOCAL, STT_SECTION
    dataSym->st_shndx = data->index;
    symtab->symbols.push_back(dataSym);
    
    textSym = new Elf64_Sym;
    textSym->st_info = 3;                   // STB_LOCAL, STT_SECTION
    textSym->st_shndx = text->index;
    symtab->symbols.push_back(textSym);
}

void Elf64File::write() {
    FILE *file = fopen(name.c_str(), "wb");
    
    // Write the header
    header->e_shnum = sections.size();
    header->e_shstrndx = 1;
    fwrite(header, sizeof(Elf64_Header), 1, file);
    
    codeOffset = (sections.size() + 1) * 64;
    
    //
    // Update the section headers
    //
    // -> section string table
    int size = 1;
    for (std::string entry : shstrtable->table) size += entry.length() + 1;
    shstrtable->header->sh_offset = Elf64_Off(codeOffset);
    shstrtable->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    // -> regular string table
    size = 1;
    for (std::string entry : strtab->table) size += entry.length() + 1;
    strtab->header->sh_offset = Elf64_Off(codeOffset);
    strtab->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    // -> symtab
    symtabSort();
    size = sizeof(Elf64_Sym) * symtab->symbols.size();
    symtab->header->sh_offset = Elf64_Off(codeOffset);
    symtab->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    int startPos = getStringPos("_start");
    for (int i = 0; i<symtab->symbols.size(); i++) {
        auto sym = symtab->symbols.at(i);
        if (sym->st_name == startPos) {
            symtab->header->sh_info = i;
            break;
        }
    }
    
    // -> .rela_text
    size = sizeof(Elf64_Rela) * rela_text->symbols.size();
    rela_text->header->sh_offset = Elf64_Off(codeOffset);
    rela_text->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    // -> data
    int dataOffset = codeOffset;                        // We have to run an update on .rela_text for this
    size = data->data.size();
    data->header->sh_offset = Elf64_Off(codeOffset);
    data->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    // -> text
    size = text->code.size();
    text->header->sh_offset = Elf64_Off(codeOffset);
    text->header->sh_size = Elf64_Xword(size);
    codeOffset += size;
    
    // Write the section headers
    for (auto section : sections) fwrite(section, sizeof(Elf64_Shdr), 1, file);
    
    //
    // Start writing data
    //
    // -> section string table
    fputc(0, file);
    for (std::string entry : shstrtable->table) {
        fputs(entry.c_str(), file);
        fputc(0, file);
    }
    
    // -> regular string table
    fputc(0, file);
    for (std::string entry : strtab->table) {
        fputs(entry.c_str(), file);
        fputc(0, file);
    }
    
    // -> symtab
    for (auto sym : symtab->symbols) fwrite(sym, sizeof(Elf64_Sym), 1, file);
    
    // -> rela.text
    for (auto sym : rela_text->symbols) {
        sym->r_addend += dataOffset;
        fwrite(sym, sizeof(Elf64_Rela), 1, file);
    }
    
    // -> data
    for (auto byte : data->data) fputc(byte, file);
    
    // -> text
    for (auto byte : text->code) fputc(byte, file);
    
    
    
    fclose(file);
}

void Elf64File::addFunctionSymbol(std::string name, int location, bool isGlobal) {
    strtab->table.push_back(name);
    int pos = getStringPos(name);
    
    Elf64_Sym *symbol = new Elf64_Sym;
    symbol->st_name = pos;
    if (isGlobal) symbol->st_info = ELF64_ST_INFO(STB_GLOBAL, STT_NOTYPE);
    else symbol->st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol->st_shndx = text->index;
    symbol->st_value = Elf64_Addr(location);
    symtab->symbols.push_back(symbol);
}

void Elf64File::addDataSymbol(std::string name, int location) {
    strtab->table.push_back(name);
    int pos = getStringPos(name);
    
    Elf64_Sym *symbol = new Elf64_Sym;
    symbol->st_name = pos;
    symbol->st_info = ELF64_ST_INFO(STB_LOCAL, STT_NOTYPE);
    symbol->st_shndx = data->index;
    symbol->st_value = Elf64_Addr(location);
    symtab->symbols.push_back(symbol);
}

void Elf64File::addDataRef(std::string name) {

}

void Elf64File::addDataStr(std::string str) {
    for (int i = 0; i<str.length(); i++) data->data.push_back(str[i]);
    data->data.push_back(0);
}

void Elf64File::addCode8(uint8_t code) {
    text->code.push_back(code);
}

void Elf64File::addCode16(uint16_t code) {
    text->code.push_back(code);
    text->code.push_back(code << 8);
}

void Elf64File::addCode32(uint32_t code) {
    text->code.push_back(code);
    text->code.push_back(code << 8);
    text->code.push_back(code << 16);
    text->code.push_back(code << 24);
}

int Elf64File::getSectionNamePos(std::string name) {
    int pos = 1;
    
    for (std::string n : shstrtable->table) {
        if (n == name) break;
        pos += n.length() + 1;
    }
    
    return pos;
}

int Elf64File::getStringPos(std::string name) {
    int pos = 1;
    
    for (std::string n : strtab->table) {
        if (n == name) break;
        pos += n.length() + 1;
    }
    
    return pos;
}

void Elf64File::symtabSort() {
    std::vector<Elf64_Sym *> local, global;
    
    for (auto sym : symtab->symbols) {
        if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL) global.push_back(sym);
        else local.push_back(sym);
    }
    
    symtab->symbols.clear();
    for (auto sym : local) symtab->symbols.push_back(sym);
    for (auto sym : global) symtab->symbols.push_back(sym);
}

