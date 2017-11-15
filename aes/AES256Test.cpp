// g++ -g -o AES256Test AES256.cpp AES256Test.cpp -lcrypto
#include <stdio.h>
#include <openssl/rand.h>
#include <string>
#include "AES256.h"

using namespace std;

int WriteStr(const void *data, size_t len, void* ptr) {
    string* str = (string*)ptr;
    str->append((const char*)data, len);
    return len;
}

class AES256FileEncrypt : public AES256Encrypt {
public:
    void SetFile(FILE* f) {
        f_ = f;
    }

    void OnCipherTextOutput(unsigned char* cipherText, int len) {
        fwrite(cipherText, 1, len, f_);
    }

private:
    FILE* f_;
};

class AES256FileDecrypt : public AES256Decrypt {
public:
    void SetFile(FILE* f) {
        f_ = f;
    }

    void OnPlainTextOutput(unsigned char* plainText, int len) {
        fwrite(plainText, 1, len, f_);
    }

private:
    FILE* f_;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n"
                "  %s [file]\n", argv[0]);
        return 1;
    }
    string oriFile = argv[1];
    string encryptFile = oriFile + ".encrypt";
    string decryptFile = encryptFile + ".decrypt";

    unsigned char key[KEY_LEN];
    RAND_bytes(key, KEY_LEN);
    string keyStr;
    BIO_dump_cb(WriteStr, &keyStr, (const char *)key, KEY_LEN);
    printf("Key:\n%s", keyStr.c_str());

    unsigned char iv[IV_LEN];
    RAND_bytes(iv, IV_LEN);
    string ivStr;
    BIO_dump_cb(WriteStr, &ivStr, (const char *)iv, IV_LEN);
    printf("IV:\n%s", ivStr.c_str());

    FILE* oriF = fopen(oriFile.c_str(), "rb");
    FILE* encryptF = fopen(encryptFile.c_str(), "wb");
    if (!oriF || !encryptF) {
      printf("Can't open file\n");
      return 1;
    }

    AES256FileEncrypt fileEncrypt;
    fileEncrypt.Init(key, iv);
    fileEncrypt.SetFile(encryptF);

    int n;
    unsigned char buf[10000];
    while((n = fread(buf, 1, 10000, oriF)) > 0) {
        if (!fileEncrypt.PlainTextInput(buf, n)) {
            printf("File encrypt fail\n");
            return 1;
        }
    }
    if (!fileEncrypt.PlainTextFinish()) {
        printf("File encrypt fail\n");
        return 1;
    }
    fclose(oriF);
    fclose(encryptF);

    encryptF = fopen(encryptFile.c_str(), "rb");
    FILE* decryptF = fopen(decryptFile.c_str(), "wb");
    if (!encryptF || !decryptF) {
      printf("Can't open file\n");
      return 1;
    }

    AES256FileDecrypt fileDecrypt;
    fileDecrypt.Init(key, iv);
    fileDecrypt.SetFile(decryptF);

    while((n = fread(buf, 1, 10000, encryptF)) > 0) {
        if (!fileDecrypt.CipherTextInput(buf, n)) {
            printf("File decrypt fail\n");
            return 1;
        }
    }
    if (!fileDecrypt.CipherTextFinish()) {
        printf("File decrypt fail\n");
        return 1;
    }
    fclose(encryptF);
    fclose(decryptF);
    return 0;
}

