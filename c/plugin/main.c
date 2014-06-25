#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include "interface.h"

size_t Output(unsigned char* data, size_t len) {
    return write(STDOUT_FILENO, data, len);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s [so file]\n", argv[0]);
        return 0;
    }

    char* sofile = argv[1];
    void* handle = dlopen(sofile, RTLD_LAZY);
    if (!handle) {
        printf("Can't open %s: %s\n", sofile, dlerror());
        return 0;
    }

    InitFunc initFunc = NULL;
    *(void**)(&initFunc) = dlsym(handle, "Init");
    if (!initFunc) {
        printf("Can't locate function Init: %s\n", dlerror());
        return 0;
    }

    OnDataFunc onData = NULL;
    void* context = NULL;
    initFunc(Output, &onData, &context);

    unsigned char buf[1024];
    ssize_t len;
    while((len = read(STDIN_FILENO, buf, 1024)) > 0) {
        onData(context, buf, len);
    }

    dlclose(handle);
    return 0;
}

