#ifndef BASE64_H
#define BASE64_H

#include <openssl/evp.h>

#define INPUT_LEN   2048
#define OUTPUT_LEN  4096

class BASE64Encoder {
public:
    BASE64Encoder();
    ~BASE64Encoder();

    void Encode(unsigned char* data, int len);
    void EncodeFinish();

    virtual void OnOutput(unsigned char* encodedData, int len) = 0;

private:
    EVP_ENCODE_CTX encodeCtx_;
    unsigned char* outputBuf_;
};

class BASE64Decoder {
public:
    BASE64Decoder();
    ~BASE64Decoder();

    bool Decode(unsigned char* data, int len);
    bool DecodeFinish();

    virtual void OnOutput(unsigned char* decodedData, int len) = 0;

private:
    EVP_ENCODE_CTX encodeCtx_;
    unsigned char* outputBuf_;
};

#endif

