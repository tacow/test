#include <stdio.h>
#include <zlib.h>

#define BUF_SIZE (1024 * 1024)

char g_inData[BUF_SIZE];
unsigned long g_inDataLen;

char g_outData[BUF_SIZE];
unsigned long g_outDataLen;

void ReadFile(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Can't open file: %s\n", filename);
        return;
    }
    g_inDataLen = fread(g_inData, 1, BUF_SIZE, f);
    fclose(f);
}

void WriteFile(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Can't open file: %s\n", filename);
        return;
    }
    fwrite(g_outData, 1, g_outDataLen, f);
    fclose(f);
}

void CompressFile(const char* filename) {
    ReadFile(filename);

    g_outDataLen = BUF_SIZE;
    if (Z_OK != compress(g_outData, &g_outDataLen, g_inData, g_inDataLen)) {
        printf("Compress failed\n");
        return;
    }

    char filename2[256];
    snprintf(filename2, 256, "%s.compress", filename);
    WriteFile(filename2);
}

void UncompressFile(const char* filename) {
    ReadFile(filename);

    g_outDataLen = BUF_SIZE;
    if (Z_OK != uncompress(g_outData, &g_outDataLen, g_inData, g_inDataLen)) {
        printf("Uncompress failed\n");
        return;
    }

    char filename2[256];
    snprintf(filename2, 256, "%s.uncompress", filename);
    WriteFile(filename2);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s <-d> [filename]\n", argv[0]);
        return 0;
    }

    if (argc > 2 && strcmp(argv[1], "-d") == 0)
        UncompressFile(argv[2]);
    else
        CompressFile(argv[1]);

    return 0;
}

