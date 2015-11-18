#ifndef TOKEN_READER_H
#define TOKEN_READER_H

class TokenReader {
public:
    TokenReader();
    ~TokenReader();

    void Init(char* str, const char* delim);
    void CopyInit(const char* str, const char* delim);

    char* ReadToken();

private:
    char* m_str;
    const char* m_delim;

    bool  m_isCopied;

    bool  m_firstCall;
    char* m_saveptr;
};

#endif

