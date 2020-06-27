#include "string_append.h"
#include <stdio.h>

int main() {
    char buf[20] = "";
    string_append(buf, sizeof(buf), "abc %d ", 123);
    string_append(buf, sizeof(buf), "def %d ", 456);
    string_append(buf, sizeof(buf), "ghi %d ", 789);
    string_append(buf, sizeof(buf), "finish");
    printf("%s\n", buf);
    return 0;
}

