#include "AES256.h"
#include <algorithm>

using namespace std;

AES256Encrypt::AES256Encrypt() {
    cipherCtx_ = EVP_CIPHER_CTX_new();
    outputBuf_ = new unsigned char[OUTPUT_LEN];
}

AES256Encrypt::~AES256Encrypt() {
    EVP_CIPHER_CTX_free(cipherCtx_);
    delete[] outputBuf_;
}

void AES256Encrypt::Init(unsigned char key[KEY_LEN], unsigned char iv[IV_LEN]) {
    EVP_EncryptInit_ex(cipherCtx_, EVP_aes_256_cbc(), NULL, key, iv);
}

bool AES256Encrypt::PlainTextInput(unsigned char* plainText, int len) {
    int pos = 0;
    while (len) {
        int encryptLen = min(len, INPUT_LEN);
        int outputLen = 0;
        if (!EVP_EncryptUpdate(cipherCtx_, outputBuf_, &outputLen, &plainText[pos], encryptLen))
            return false;
        if (outputLen)
            OnCipherTextOutput(outputBuf_, outputLen);
        pos += encryptLen;
        len -= encryptLen;
    }
    return true;
}

bool AES256Encrypt::PlainTextFinish() {
    int outputLen = 0;
    if (!EVP_EncryptFinal_ex(cipherCtx_, outputBuf_, &outputLen))
        return false;
    if (outputLen)
        OnCipherTextOutput(outputBuf_, outputLen);
    return true;
}

AES256Decrypt::AES256Decrypt() {
    cipherCtx_ = EVP_CIPHER_CTX_new();
    outputBuf_ = new unsigned char[OUTPUT_LEN];
}

AES256Decrypt::~AES256Decrypt() {
    EVP_CIPHER_CTX_free(cipherCtx_);
    delete[] outputBuf_;
}

void AES256Decrypt::Init(unsigned char key[KEY_LEN], unsigned char iv[IV_LEN]) {
    EVP_DecryptInit_ex(cipherCtx_, EVP_aes_256_cbc(), NULL, key, iv);
}

bool AES256Decrypt::CipherTextInput(unsigned char* cipherText, int len) {
    int pos = 0;
    while (len) {
        int decryptLen = min(len, INPUT_LEN);
        int outputLen = 0;
        if (!EVP_DecryptUpdate(cipherCtx_, outputBuf_, &outputLen, &cipherText[pos], decryptLen))
            return false;
        if (outputLen)
            OnPlainTextOutput(outputBuf_, outputLen);
        pos += decryptLen;
        len -= decryptLen;
    }
    return true;
}

bool AES256Decrypt::CipherTextFinish() {
    int outputLen = 0;
    if (!EVP_DecryptFinal_ex(cipherCtx_, outputBuf_, &outputLen))
        return false;
    if (outputLen)
        OnPlainTextOutput(outputBuf_, outputLen);
    return true;
}

