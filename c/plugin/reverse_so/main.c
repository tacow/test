#include <stdlib.h>
#include <string.h>
#include "interface.h"

#define SEP ' '

struct Context {
    char data[1024];
    int len;

    OutputFunc outputFunc;
};

void Reverse(char* str, size_t len) {
    size_t half = len / 2;
    size_t i;
    for(i = 0; i < half; ++i) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void OnData(void* context, unsigned char* data, size_t len) {
    struct Context* c = (struct Context*)context;
    memcpy(c->data + c->len, data, len);
    c->len += len;
    char* begin = c->data;
    char* end = NULL;
    while((end = memchr(begin, SEP, c->len - (begin - c->data)))) {
        int wordLen = end - begin;
        Reverse(begin, wordLen);
        c->outputFunc((unsigned char*)begin, wordLen + 1);
        begin = end + 1;
    }
    c->len -= begin - c->data;
    memmove(c->data, begin, c->len);
}

void Init(OutputFunc output, OnDataFunc* onData, void** context) {
    struct Context* c = malloc(sizeof(struct Context));
    c->len = 0;
    c->outputFunc = output;

    *onData = OnData;
    *context = c;
}

