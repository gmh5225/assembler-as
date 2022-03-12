#pragma once

#include <stdint.h>
#include <string>
#include <vector>

// All the ELF types
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef	int32_t  Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef	int64_t  Elf64_Sxword;
typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Section;
typedef Elf64_Half Elf64_Versym;

#define EI_NIDENT (16)

#define SHN_ABS 65521

#define ELF32_ST_BIND(val)		(((unsigned char) (val)) >> 4)
#define ELF32_ST_TYPE(val)		((val) & 0xf)
#define ELF32_ST_INFO(bind, type)	(((bind) << 4) + ((type) & 0xf))

/* Both Elf32_Sym and Elf64_Sym use the same one-byte st_info field.  */
#define ELF64_ST_BIND(val)		ELF32_ST_BIND (val)
#define ELF64_ST_TYPE(val)		ELF32_ST_TYPE (val)
#define ELF64_ST_INFO(bind, type)	ELF32_ST_INFO ((bind), (type))
#define ELF64_R_INFO(sym,type)		((((Elf64_Xword) (sym)) << 32) + (type))

#define STB_LOCAL	0		/* Local symbol */
#define STB_GLOBAL	1		/* Global symbol */
#define STT_NOTYPE	0		/* Symbol type is unspecified */
#define STT_OBJECT	1		/* Symbol is a data object */
#define STT_FUNC	2		/* Symbol is a code object */
#define STT_SECTION	3		/* Symbol associated with a section */
#define STT_FILE	4		/* Symbol's name is file name */

// Raw ELF64 headers
// These can be directly written to a binary file
//
struct Elf64_Header {
    unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
    Elf64_Half	e_type;			/* Object file type */
    Elf64_Half	e_machine;		/* Architecture */
    Elf64_Word	e_version;		/* Object file version */
    Elf64_Addr	e_entry;		/* Entry point virtual address */
    Elf64_Off	e_phoff;		/* Program header table file offset */
    Elf64_Off	e_shoff;		/* Section header table file offset */
    Elf64_Word	e_flags;		/* Processor-specific flags */
    Elf64_Half	e_ehsize;		/* ELF header size in bytes */
    Elf64_Half	e_phentsize;		/* Program header table entry size */
    Elf64_Half	e_phnum;		/* Program header table entry count */
    Elf64_Half	e_shentsize;		/* Section header table entry size */
    Elf64_Half	e_shnum;		/* Section header table entry count */
    Elf64_Half	e_shstrndx;		/* Section header string table index */
};

struct Elf64_Shdr {
    Elf64_Word	sh_name = 0;		/* Section name (string tbl index) */
    Elf64_Word	sh_type = 0;		/* Section type */
    Elf64_Xword	sh_flags = 0;		/* Section flags */
    Elf64_Addr	sh_addr = 0;		/* Section virtual addr at execution */
    Elf64_Off	sh_offset = 0;		/* Section file offset */
    Elf64_Xword	sh_size = 0;		/* Section size in bytes */
    Elf64_Word	sh_link = 0;		/* Link to another section */
    Elf64_Word	sh_info = 0;		/* Additional section information */
    Elf64_Xword	sh_addralign = 0;		/* Section alignment */
    Elf64_Xword	sh_entsize = 0;		/* Entry size if section holds table */
};

struct Elf64_Sym {
    Elf64_Word	st_name = 0;		/* Symbol name (string tbl index) */
    unsigned char	st_info = 0;		/* Symbol type and binding */
    unsigned char st_other = 0;		/* Symbol visibility */
    Elf64_Section	st_shndx = 0;		/* Section index */
    Elf64_Addr	st_value = 0;		/* Symbol value */
    Elf64_Xword	st_size = 0;		/* Symbol size */
};

struct Elf64_Rela {
  Elf64_Addr	r_offset;		/* Address */
  Elf64_Xword	r_info;			/* Relocation type and symbol index */
  Elf64_Sxword	r_addend;		/* Addend */
};

// Abstractions of other section headers
struct ElfSectionStrTable {
    Elf64_Shdr *header;
    std::vector<std::string> table;
    int index;
};

struct ElfStrTable {
    Elf64_Shdr *header;
    std::vector<std::string> table;
    int index;
};

struct ElfSymTable {
    Elf64_Shdr *header;
    std::vector<Elf64_Sym *> symbols;
    int index;
};

struct ElfData {
    Elf64_Shdr *header;
    std::vector<uint8_t> data;
    int index;
};

struct ElfText {
    Elf64_Shdr *header;
    std::vector<uint8_t> code;
    int index;
};

struct ElfRelaText {
    Elf64_Shdr *header;
    std::vector<Elf64_Rela *> symbols;
    int index;
};

// Represents an ELF file
class Elf64File {
public:
    explicit Elf64File(std::string name);
    void write();
    
    void addDataStr(std::string str);
    void addFunctionSymbol(std::string name, int location, bool isGlobal);
    void addDataSymbol(std::string name, int location);
    void addDataRef(int codeOffset, int dataOffset);
    void addCode8(uint8_t code);
    void addCode16(uint16_t code);
    void addCode32(uint32_t code);
private:
    // Utility functions
    int getSectionNamePos(std::string name);
    int getStringPos(std::string name);
    void symtabSort();
    
    // Variables
    std::string name = "";
    int codeOffset = 0;
    
    Elf64_Header *header;
    ElfSectionStrTable *shstrtable;
    ElfStrTable *strtab;
    ElfSymTable *symtab;
    ElfData *data;
    ElfText *text;
    Elf64_Sym *textSym;
    ElfRelaText *rela_text;
    
    std::vector<Elf64_Shdr *> sections;
};

