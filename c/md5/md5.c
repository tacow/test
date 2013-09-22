#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

void FileMd5(const char* filename) {
    FILE* f = NULL;
    char buf[4096];
    size_t n = 0;
    MD5_CTX ctx;
    unsigned char md[MD5_DIGEST_LENGTH];
    size_t i = 0;

    f = fopen(filename, "r");
    if (f == NULL) {
        printf("Can't open file\n");
        return;
    }

    MD5_Init(&ctx);
    while((n = fread(buf, 1, 4096, f)) > 0) {
        MD5_Update(&ctx, buf, n);
    }
    MD5_Final(md, &ctx);

    for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
        printf("%02X", md[i]);
    printf("\n");
    fclose(f);
}

void StringMd5() {
    char line[4096];
    unsigned char md[MD5_DIGEST_LENGTH];
    size_t i = 0;

    fgets(line, 4096, stdin);
    MD5((unsigned char*)line, strlen(line), md);

    for(i = 0; i < MD5_DIGEST_LENGTH; ++i)
        printf("%02X", md[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    char* filename = NULL;

    if (argc < 2) {
        printf("%s [filename] OR\n", argv[0]);
        printf("%s -\n", argv[0]);
        return 0;
    }

    filename = argv[1];
    if (0 == strcmp(filename, "-"))
        StringMd5();
    else
        FileMd5(filename);
    return 0;
}

