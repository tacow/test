#include "StreamReader.h"

StreamReader::StreamReader() {
    m_dataBufPtr = NULL;
    m_bytesLeft = 0;
}

StreamReader::~StreamReader() {
}

void StreamReader::Init(const void* dataBuf, size_t bufLen) {
    m_dataBufPtr = (const char*)dataBuf;
    m_bytesLeft = bufLen;
}

size_t StreamReader::Read(void* buf, size_t len) {
    size_t bytesRead = min(len, m_bytesLeft);
    memcpy(buf, m_dataBufPtr, bytesRead);
    m_dataBufPtr += bytesRead;
    m_bytesLeft -= bytesRead;
    return bytesRead;
}

bool StreamReader::Empty() {
    return m_bytesLeft == 0;
}

