#include <stdlib.h>
#include <string.h>
#include "interface.h"

#define SEP ' '

struct Context {
    char data[1024];
    int len;

    OutputFunc outputFunc;
};

void OnData(void* context, unsigned char* data, size_t len) {
    struct Context* c = (struct Context*)context;
    memcpy(c->data + c->len, data, len);
    c->len += len;
    char* begin = c->data;
    char* end = NULL;
    while((end = memchr(begin, SEP, c->len - (begin - c->data)))) {
        int wordLen = end - begin;
        c->outputFunc((unsigned char*)begin, wordLen + 1);
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

