#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <unicode/utypes.h>
#include <string>

using namespace std;

typedef basic_string<UChar> ustring;

int StrWidth(const ustring& str);
void RestrainStr(ustring& str, int restrainWidth);

#endif

