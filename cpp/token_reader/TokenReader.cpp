#include "TokenReader.h"
#include <string.h>

TokenReader::TokenReader() {
    m_str = NULL;
    m_delim = NULL;
    m_isCopied = false;
    m_firstCall = true;
    m_saveptr = NULL;
}

TokenReader::~TokenReader() {
    if (m_isCopied)
        delete[] m_str;
}

void TokenReader::Init(char* str, const char* delim) {
    m_str = str;
    m_delim = delim;
}

void TokenReader::CopyInit(const char* str, const char* delim) {
    size_t len = strlen(str) + 1;
    m_str = new char[len];
    memcpy(m_str, str, len);
    m_delim = delim;
    m_isCopied = true;
}

char* TokenReader::ReadToken() {
    if (m_firstCall) {
        m_firstCall = false;
        return strtok_r(m_str, m_delim, &m_saveptr);
    } else {
        return strtok_r(NULL, m_delim, &m_saveptr);
    }
}

