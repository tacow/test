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

void GzCompressFile(const char* filename) {
    ReadFile(filename);

    char filename2[256];
    snprintf(filename2, 256, "%s.gz", filename);
    gzFile gzf = gzopen(filename2, "wb");
    if (!gzf) {
        printf("Can't open file: %s\n", filename2);
        return;
    }
    gzwrite(gzf, g_inData, g_inDataLen);
    gzclose(gzf);
}

void GzUncompressFile(const char* filename) {
    gzFile gzf = gzopen(filename, "rb");
    if (!gzf) {
        printf("Can't open file: %s\n", filename);
        return;
    }
    g_outDataLen = gzread(gzf, g_outData, BUF_SIZE);
    gzclose(gzf);

    char filename2[256];
    snprintf(filename2, 256, "%s.ungz", filename);
    WriteFile(filename2);
}

int main(int argc, char* argv[]) {
    const char* filename = NULL;
    int uncompress = 0;
    int gzipFormat = 0;

    int opt = getopt(argc, argv, "ug");
    while(-1 != opt) {
        switch(opt) {
        case 'u':
            uncompress = 1;
            break;
        case 'g':
            gzipFormat = 1;
            break;
        }
        opt = getopt(argc, argv, "ug");
    }
    if (optind >= argc) {
        printf("Usage: %s [-u] [-g] [filename]\n", argv[0]);
        printf("  -u: uncompress\n");
        printf("  -g: gzip format (default: zlib format)\n");
        return 0;
    }
    filename = argv[optind];

    if (gzipFormat) {
        if (uncompress)
            GzUncompressFile(filename);
        else
            GzCompressFile(filename);
    } else {
        if (uncompress)
            UncompressFile(filename);
        else
            CompressFile(filename);
    }

    return 0;
}

