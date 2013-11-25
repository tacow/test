#include <stdio.h>
#include "CharsetConvert.h"

void WriteFile(const char* filename, const string& content) {
    FILE* f = fopen(filename, "w");
    fwrite(content.c_str(), 1, content.size(), f);
    fclose(f);
}

int main() {
    CharsetConvert convertor;

    if (!convertor.Init("UTF-8", "GB18030")) {
        printf("Convertor initialize failed\n");
        return 0;
    }

    string utf8Str;
    string gbStr = "���\n���\n";
    convertor.Convert(gbStr, utf8Str);
    WriteFile("hello_utf8.txt", utf8Str);

    gbStr = "��ӭ\n��ӭ\n";
    convertor.Convert(gbStr, utf8Str);
    WriteFile("welcome_utf8.txt", utf8Str);

    return 0;
}

