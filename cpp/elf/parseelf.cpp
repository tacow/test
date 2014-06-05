#include <stdio.h>
#include <elf.h>
#include <string>

using namespace std;

void PrintSectionLine(const char* name, const char* type, const char* flags, const char* offset, const char* size) {
    printf("%-25s%-15s%-35s%10s%10s\n", name, type, flags, offset, size);
}

const char* GetSectionType(Elf64_Word sType) {
    switch(sType) {
    case SHT_NULL:
        return "SHT_NULL";
    case SHT_PROGBITS:
        return "SHT_PROGBITS";
    case SHT_SYMTAB:
        return "SHT_SYMTAB";
    case SHT_STRTAB:
        return "SHT_STRTAB";
    case SHT_RELA:
        return "SHT_RELA";
    case SHT_HASH:
        return "SHT_HASH";
    case SHT_DYNAMIC:
        return "SHT_DYNAMIC";
    case SHT_NOTE:
        return "SHT_NOTE";
    case SHT_NOBITS:
        return "SHT_NOBITS";
    case SHT_REL:
        return "SHT_REL";
    case SHT_SHLIB:
        return "SHT_SHLIB";
    case SHT_DYNSYM:
        return "SHT_DYNSYM";
    case SHT_INIT_ARRAY:
        return "SHT_INIT_ARRAY";
    case SHT_FINI_ARRAY:
        return "SHT_FINI_ARRAY";
    case SHT_PREINIT_ARRAY:
        return "SHT_PREINIT_ARRAY";
    case SHT_GROUP:
        return "SHT_GROUP";
    case SHT_SYMTAB_SHNDX:
        return "SHT_SYMTAB_SHNDX";
    case SHT_NUM:
        return "SHT_NUM";
    case SHT_LOOS:
        return "SHT_LOOS";
    case SHT_GNU_ATTRIBUTES:
        return "SHT_GNU_ATTRIBUTES";
    case SHT_GNU_HASH:
        return "SHT_GNU_HASH";
    case SHT_GNU_LIBLIST:
        return "SHT_GNU_LIBLIST";
    case SHT_CHECKSUM:
        return "SHT_CHECKSUM";
    case SHT_GNU_verdef:
        return "SHT_GNU_verdef";
    case SHT_GNU_verneed:
        return "SHT_GNU_verneed";
    case SHT_GNU_versym:
        return "SHT_GNU_versym";
    case SHT_LOPROC:
        return "SHT_LOPROC";
    case SHT_HIPROC:
        return "SHT_HIPROC";
    case SHT_LOUSER:
        return "SHT_LOUSER";
    case SHT_HIUSER:
        return "SHT_HIUSER";
    }
    return "";
}

string GetSectionFlags(Elf64_Xword sFlags) {
    string strFlags;
    if (sFlags & SHF_WRITE)
        strFlags += "SHF_WRITE ";
    if (sFlags & SHF_ALLOC)
        strFlags += "SHF_ALLOC ";
    if (sFlags & SHF_EXECINSTR)
        strFlags += "SHF_EXECINSTR ";
    if (sFlags & SHF_MERGE)
        strFlags += "SHF_MERGE ";
    if (sFlags & SHF_STRINGS)
        strFlags += "SHF_STRINGS ";
    if (sFlags & SHF_INFO_LINK)
        strFlags += "SHF_INFO_LINK ";
    if (sFlags & SHF_LINK_ORDER)
        strFlags += "SHF_LINK_ORDER ";
    if (sFlags & SHF_OS_NONCONFORMING)
        strFlags += "SHF_OS_NONCONFORMING ";
    if (sFlags & SHF_GROUP)
        strFlags += "SHF_GROUP ";
    if (sFlags & SHF_TLS)
        strFlags += "SHF_TLS ";
    if (sFlags & SHF_MASKOS)
        strFlags += "SHF_MASKOS ";
    if (sFlags & SHF_MASKPROC)
        strFlags += "SHF_MASKPROC ";
    if (sFlags & SHF_ORDERED)
        strFlags += "SHF_ORDERED ";
    if (sFlags & SHF_EXCLUDE)
        strFlags += "SHF_EXCLUDE ";
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

    PrintSectionLine("[Section name]", "[Type]", "[Flags]", "[Offset]", "[Size]");

    fseek(f, elf.e_shoff, SEEK_SET);
    for(Elf64_Half i = 0; i < elf.e_shnum; ++i) {
        Elf64_Shdr shdr;
        fread(&shdr, 1, sizeof(Elf64_Shdr), f);

        char strOffset[16], strSize[16];
        snprintf(strOffset, 16, "%lX", shdr.sh_offset);
        snprintf(strSize, 16, "%lX", shdr.sh_size);

        PrintSectionLine(&sstContent[shdr.sh_name], GetSectionType(shdr.sh_type), GetSectionFlags(shdr.sh_flags).c_str(), strOffset, strSize);
    }

    delete[] sstContent;
    fclose(f);
}

