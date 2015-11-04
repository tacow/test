#include "StreamWriter.h"

StreamWriter::StreamWriter() {
    m_dataBufPtr = NULL;
    m_dataSize = 0;
    m_bytesLeft = 0;
}

StreamWriter::~StreamWriter() {
}

void StreamWriter::Init(void* dataBuf, size_t bufLen) {
    m_dataBufPtr = (char*)dataBuf;
    m_dataSize = 0;
    m_bytesLeft = bufLen;
}

size_t StreamWriter::Write(const void* buf, size_t len) {
    size_t bytesWriten = min(len, m_bytesLeft);
    memcpy(m_dataBufPtr, buf, bytesWriten);
    m_dataBufPtr += bytesWriten;
    m_dataSize += bytesWriten;
    m_bytesLeft -= bytesWriten;
    return bytesWriten;
}

size_t StreamWriter::GetSize() {
    return m_dataSize;
}

