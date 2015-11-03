#include <string.h>
#include <ctype.h>

char* Trim(char* str) {
    while(*str && isspace(*str))
        ++str;

    size_t len = strlen(str);
    size_t i;
    for(i = len; i > 0; --i) {
        if (!isspace(str[i - 1])) {
            str[i] = '\0';
            break;
        }
    }
    return str;
}

// Example
#include <stdio.h>

void Test(const char* str) {
    char buf[256];
    strcpy(buf, str);
    printf("\"%s\"\n", Trim(buf));
}

int main() {
    Test("  \t \n abc def \r \n ");
    Test("  ");
    Test("");
    return 0;
}

