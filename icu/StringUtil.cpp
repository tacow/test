#include "StringUtil.h"
#include "CharIterator.h"

int StrWidth(const ustring& str) {
    int strWidth = 0;
    CharIterator ci;
    ci.SetString(str.c_str(), str.size());
    int chLen, chWidth;
    while (ci.GetNextChar(&chLen, &chWidth)) {
        if (chWidth > 0)
            strWidth += chWidth;
    }
    return strWidth;
}

void RestrainStr(ustring& str, int restrainWidth) {
    int strWidth = 0;
    ustring restrainedStr;
    CharIterator ci;
    ci.SetString(str.c_str(), str.size());
    const UChar* ch;
    int chLen, chWidth;
    while ((ch = ci.GetNextChar(&chLen, &chWidth))) {
        if (chWidth < 0)
            continue;
        if (strWidth + chWidth > restrainWidth) {
            str = restrainedStr;
            return;
        }
        restrainedStr.append(ch, chLen);
        strWidth += chWidth;
    }
}

