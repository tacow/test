#ifndef STREAM_READER_H
#define STREAM_READER_H

#include <string.h>
#include <algorithm>

using namespace std;

class StreamReader {
public:
    StreamReader();
    ~StreamReader();

    void Init(const void* dataBuf, size_t bufLen);

    template<class T>
    bool Read(T& val) {
        size_t valSize = sizeof(val);
        return valSize == Read(&val, valSize);
    }

    size_t Read(void* buf, size_t len);

    bool Empty();
private:
    const char* m_dataBufPtr;
    size_t m_bytesLeft;
};

#endif

