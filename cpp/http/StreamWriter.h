#ifndef STREAM_WRITER_H
#define STREAM_WRITER_H

#include <string.h>
#include <algorithm>

using namespace std;

class StreamWriter {
public:
    StreamWriter();
    ~StreamWriter();

    void Init(void* dataBuf, size_t bufSize);

    template<class T>
    bool Write(T& val) {
        size_t valSize = sizeof(val);
        return valSize == Write(&val, valSize);
    }

    size_t Write(const void* buf, size_t len);

    size_t GetSize();

private:
    char* m_dataBufPtr;
    size_t m_dataSize;
    size_t m_bytesLeft;
};

#endif

