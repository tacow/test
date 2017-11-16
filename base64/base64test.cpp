// g++ -g -o base64test base64.cpp base64test.cpp -lcrypto

#include <stdio.h>
#include <string.h>
#include <string>
#include "base64.h"

using namespace std;

class BASE64FileEncoder : public BASE64Encoder {
    public:
        void SetFile(FILE* f) {
            f_ = f;
        }

        void OnOutput(unsigned char* encodedData, int len) {
            fwrite(encodedData, 1, len, f_);
        }

    private:
        FILE* f_;
};

class BASE64FileDecoder : public BASE64Decoder {
    public:
        void SetFile(FILE* f) {
            f_ = f;
        }

        void OnOutput(unsigned char* decodedData, int len) {
            fwrite(decodedData, 1, len, f_);
        }

    private:
        FILE* f_;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage:\n"
                "  %s [file]       -- BASE64 encode\n"
                "  %s -d [file]    -- BASE64 decode\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-d") != 0) {
        string oriFile = argv[1];
        string encodedFile = oriFile + ".b64encode";

        FILE* oriF = fopen(oriFile.c_str(), "rb");
        FILE* encodedF = fopen(encodedFile.c_str(), "wb");
        if (!oriF || !encodedF) {
            printf("Can't open file\n");
            return 1;
        }

        BASE64FileEncoder fileEncoder;
        fileEncoder.SetFile(encodedF);

        int n;
        unsigned char buf[10005];
        while((n = fread(buf, 1, 10005, oriF)) > 0)
            fileEncoder.Encode(buf, n);
        fileEncoder.EncodeFinish();
        fclose(oriF);
        fclose(encodedF);
    } else {
        string oriFile = argv[2];
        string decodedFile = oriFile + ".b64decode";

        FILE* oriF = fopen(oriFile.c_str(), "rb");
        FILE* decodedF = fopen(decodedFile.c_str(), "wb");
        if (!oriF || !decodedF) {
            printf("Can't open file\n");
            return 1;
        }

        BASE64FileDecoder fileDecoder;
        fileDecoder.SetFile(decodedF);

        int n;
        unsigned char buf[10005];
        while((n = fread(buf, 1, 10005, oriF)) > 0) {
            if (!fileDecoder.Decode(buf, n)) {
                printf("File decode fail\n");
                return 1;
            }
        }
        if (!fileDecoder.DecodeFinish()) {
            printf("File decode fail\n");
            return 1;
        }
        fclose(oriF);
        fclose(decodedF);
    }
}

