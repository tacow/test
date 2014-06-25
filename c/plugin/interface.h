typedef void (*OnDataFunc)(void* context, unsigned char* data, size_t len);
typedef size_t (*OutputFunc)(unsigned char* data, size_t len);
typedef void (*InitFunc)(OutputFunc output, OnDataFunc* onData, void** context);
