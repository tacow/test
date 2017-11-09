#ifndef CHAR_ITERATOR_H
#define CHAR_ITERATOR_H

#include <unicode/utypes.h>

class CharIterator {
public:
    CharIterator();
    ~CharIterator();

    void SetString(const UChar* str, int len);
    const UChar* GetNextChar(int* charLen, int* charWidth);

private:
    const UChar* str_;
    int len_;
    int pos_;
};

#endif

