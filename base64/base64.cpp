#include "base64.h"
#include <algorithm>

using namespace std;

BASE64Encoder::BASE64Encoder() {
    EVP_EncodeInit(&encodeCtx_);
    outputBuf_ = new unsigned char[OUTPUT_LEN];
}

BASE64Encoder::~BASE64Encoder() {
    delete[] outputBuf_;
}

void BASE64Encoder::Encode(unsigned char* data, int len) {
    int pos = 0;
    while (len) {
        int encodeLen = min(len, INPUT_LEN);
        int outputLen = 0;
        EVP_EncodeUpdate(&encodeCtx_, outputBuf_, &outputLen, &data[pos], encodeLen);
        if (outputLen)
            OnOutput(outputBuf_, outputLen);
        pos += encodeLen;
        len -= encodeLen;
    }
}

void BASE64Encoder::EncodeFinish() {
    int outputLen = 0;
    EVP_EncodeFinal(&encodeCtx_, outputBuf_, &outputLen);
    if (outputLen)
        OnOutput(outputBuf_, outputLen);
}

BASE64Decoder::BASE64Decoder() {
    EVP_DecodeInit(&encodeCtx_);
    outputBuf_ = new unsigned char[OUTPUT_LEN];
}

BASE64Decoder::~BASE64Decoder() {
    delete[] outputBuf_;
}

bool BASE64Decoder::Decode(unsigned char* data, int len) {
    int pos = 0;
    while (len) {
        int encodeLen = min(len, OUTPUT_LEN);
        int outputLen = 0;
        if (-1 == EVP_DecodeUpdate(&encodeCtx_, outputBuf_, &outputLen, &data[pos], encodeLen))
            return false;
        if (outputLen)
            OnOutput(outputBuf_, outputLen);
        pos += encodeLen;
        len -= encodeLen;
    }
    return true;
}

bool BASE64Decoder::DecodeFinish() {
    int outputLen = 0;
    if (-1 == EVP_DecodeFinal(&encodeCtx_, outputBuf_, &outputLen))
        return false;
    if (outputLen)
        OnOutput(outputBuf_, outputLen);
    return true;
}

