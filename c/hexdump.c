#include <stdio.h>
#include <inttypes.h>

#define BUF_SIZE (1024 * 1024)

char fileData[BUF_SIZE];
size_t fileDataLen = 0;

int ReadFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f)
        return -1;
    fileDataLen = fread(fileData, 1, BUF_SIZE, f);
    fclose(f);
    return 0;
}

void HexDump(const void* p, long size) {
    const uint8_t *c = p;
    long pos = 0;
    while (size > 0) {
        long i;
        printf("%08lx: ", pos);
        for (i = 0; i < 16; i++) {
            if (i == 8)
                printf(" ");
            if (i < size)
                printf("%02x ", c[i]);
            else
                printf("   ");
        }
        printf(" ");

        for (i = 0; i < 16; i++) {
            if (i < size)
                printf("%c", c[i] >= 32 && c[i] < 127 ? c[i] : '.');
            else
                printf(" ");
        }
        printf("\n");
        c += 16;
        if (size <= 16)
            break;
        size -= 16;
        pos += 16;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [filename]\n", argv[0]);
        return 0;
    }

    if (0 != ReadFile(argv[1])) {
        printf("Can't read file: %s\n", argv[1]);
        return -1;
    }
    HexDump(fileData, fileDataLen);
    return 0;
}

