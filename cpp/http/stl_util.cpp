#include "stl_util.h"

string Trim(const string& str) {
    size_t head = 0;
    size_t tail = str.size();
    for(; head < tail; ++head)
        if (!isspace(str[head]))
            break;
    for(; tail > head; --tail)
        if (!isspace(str[tail - 1]))
            break;
    return str.substr(head, tail - head);
}

