#include <stdio.h>
#include <inttypes.h>
#include <elf.h>
#include <string>

using namespace std;

void PrintSectionLine(const char* name, const char* type, const char* flags, const char* offset, const char* size, const char* align) {
    printf("%-25s%-15s%-25s%10s%10s%10s\n", name, type, flags, offset, size, align);
}

const char* GetSectionType(Elf64_Word sType) {
    switch(sType) {
    case SHT_NULL:
        return "NULL";
    case SHT_PROGBITS:
        return "PROGBITS";
    case SHT_SYMTAB:
        return "SYMTAB";
    case SHT_STRTAB:
        return "STRTAB";
    case SHT_RELA:
        return "RELA";
    case SHT_HASH:
        return "HASH";
    case SHT_DYNAMIC:
        return "DYNAMIC";
    case SHT_NOTE:
        return "NOTE";
    case SHT_NOBITS:
        return "NOBITS";
    case SHT_REL:
        return "REL";
    case SHT_SHLIB:
        return "SHLIB";
    case SHT_DYNSYM:
        return "DYNSYM";
    case SHT_INIT_ARRAY:
        return "INIT_ARRAY";
    case SHT_FINI_ARRAY:
        return "FINI_ARRAY";
    case SHT_PREINIT_ARRAY:
        return "PREINIT_ARRAY";
    case SHT_GROUP:
        return "GROUP";
    case SHT_SYMTAB_SHNDX:
        return "SYMTAB_SHNDX";
    case SHT_NUM:
        return "NUM";
    case SHT_LOOS:
        return "LOOS";
    case SHT_GNU_ATTRIBUTES:
        return "GNU_ATTRIBUTES";
    case SHT_GNU_HASH:
        return "GNU_HASH";
    case SHT_GNU_LIBLIST:
        return "GNU_LIBLIST";
    case SHT_CHECKSUM:
        return "CHECKSUM";
    case SHT_GNU_verdef:
        return "GNU_verdef";
    case SHT_GNU_verneed:
        return "GNU_verneed";
    case SHT_GNU_versym:
        return "GNU_versym";
    case SHT_LOPROC:
        return "LOPROC";
    case SHT_HIPROC:
        return "HIPROC";
    case SHT_LOUSER:
        return "LOUSER";
    case SHT_HIUSER:
        return "HIUSER";
    }
    return "";
}

string GetSectionFlags(Elf64_Xword sFlags) {
    string strFlags;
    if (sFlags & SHF_WRITE)
        strFlags += "WRITE ";
    if (sFlags & SHF_ALLOC)
        strFlags += "ALLOC ";
    if (sFlags & SHF_EXECINSTR)
        strFlags += "EXECINSTR ";
    if (sFlags & SHF_MERGE)
        strFlags += "MERGE ";
    if (sFlags & SHF_STRINGS)
        strFlags += "STRINGS ";
    if (sFlags & SHF_INFO_LINK)
        strFlags += "INFO_LINK ";
    if (sFlags & SHF_LINK_ORDER)
        strFlags += "LINK_ORDER ";
    if (sFlags & SHF_OS_NONCONFORMING)
        strFlags += "OS_NONCONFORMING ";
    if (sFlags & SHF_GROUP)
        strFlags += "GROUP ";
    if (sFlags & SHF_TLS)
        strFlags += "TLS ";
    if (sFlags & SHF_MASKOS)
        strFlags += "MASKOS ";
    if (sFlags & SHF_MASKPROC)
        strFlags += "MASKPROC ";
    if (sFlags & SHF_ORDERED)
        strFlags += "ORDERED ";
    if (sFlags & SHF_EXCLUDE)
        strFlags += "EXCLUDE ";
    return strFlags;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s [exe file]\n", argv[0]);
        return 0;
    }
    char* exeFile = argv[1];

    Elf64_Ehdr elf;

    FILE* f = fopen(exeFile, "r");
    fread(&elf, 1, sizeof(Elf64_Ehdr), f);

    Elf64_Off sstOff = 0;
    Elf64_Xword sstSize = 0;

    fseek(f, elf.e_shoff, SEEK_SET);
    for(Elf64_Half i = 0; i < elf.e_shnum; ++i) {
        Elf64_Shdr shdr;
        fread(&shdr, 1, sizeof(Elf64_Shdr), f);

        if (i == elf.e_shstrndx) {
            sstOff = shdr.sh_offset;
            sstSize = shdr.sh_size;
        }
    }

    char* sstContent = new char[sstSize];
    fseek(f, sstOff, SEEK_SET);
    fread(sstContent, 1, sstSize, f);

    PrintSectionLine("[Section name]", "[Type]", "[Flags]", "[Offset]", "[Size]", "[Align]");

    fseek(f, elf.e_shoff, SEEK_SET);
    for(Elf64_Half i = 0; i < elf.e_shnum; ++i) {
        Elf64_Shdr shdr;
        fread(&shdr, 1, sizeof(Elf64_Shdr), f);

        char strOffset[16], strSize[16], strAlign[16];
        snprintf(strOffset, 16, "%"PRIu64, shdr.sh_offset);
        snprintf(strSize, 16, "%"PRIu64, shdr.sh_size);
        snprintf(strAlign, 16, "%"PRIu64, shdr.sh_addralign);

        PrintSectionLine(&sstContent[shdr.sh_name], GetSectionType(shdr.sh_type), GetSectionFlags(shdr.sh_flags).c_str(), strOffset, strSize, strAlign);
    }

    delete[] sstContent;
    fclose(f);
}

