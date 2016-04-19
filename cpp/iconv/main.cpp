#include <stdio.h>
#include "CharsetConvert.h"

bool ReadFile(const char* filename, string& content) {
    FILE* f = fopen(filename, "r");
    if (!f)
        return false;

    char buf[4096];
    size_t n;
    while((n = fread(buf, 1, 4096, f)))
        content.append(buf, n);
    fclose(f);
    return true;
}

bool WriteFile(const char* filename, const string& content) {
    FILE* f = fopen(filename, "w");
    if (!f)
        return false;

    fwrite(content.c_str(), 1, content.size(), f);
    fclose(f);
    return true;
}

int main(int argc, char* argv[]) {
    CharsetConvert convertor;

    if (!convertor.Init("UTF-8", "GB18030")) {
        printf("Convertor initialize failed\n");
        return 0;
    }

    for(int i = 1; i < argc; ++i) {
        string gbFilename = argv[i];
        string utf8Filename = gbFilename + ".utf8";

        string gbStr;
        string utf8Str;
        ReadFile(gbFilename.c_str(), gbStr);
        if (convertor.Convert(gbStr, utf8Str))
            WriteFile(utf8Filename.c_str(), utf8Str);
    }

    return 0;
}

