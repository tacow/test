#ifndef AES_256_H
#define AES_256_H

#include <openssl/evp.h>

#define KEY_LEN     (256 / 8)
#define BLOCK_LEN   (128 / 8)
#define IV_LEN      BLOCK_LEN
#define INPUT_LEN   4096
#define OUTPUT_LEN  (INPUT_LEN + BLOCK_LEN)

class AES256Encrypt {
public:
    AES256Encrypt();
    ~AES256Encrypt();

    void Init(unsigned char key[KEY_LEN], unsigned char iv[IV_LEN]);

    bool PlainTextInput(unsigned char* plainText, int len);
    bool PlainTextFinish();

    virtual void OnCipherTextOutput(unsigned char* cipherText, int len) = 0;

private:
    EVP_CIPHER_CTX* cipherCtx_;
    unsigned char* outputBuf_;
};

class AES256Decrypt {
public:
    AES256Decrypt();
    ~AES256Decrypt();

    void Init(unsigned char key[KEY_LEN], unsigned char iv[IV_LEN]);

    bool CipherTextInput(unsigned char* cipherText, int len);
    bool CipherTextFinish();

    virtual void OnPlainTextOutput(unsigned char* plainText, int len) = 0;

private:
    EVP_CIPHER_CTX* cipherCtx_;
    unsigned char* outputBuf_;
};

#endif

