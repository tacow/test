#include <stdio.h>
#include <string>
#include <iostream>
#include "CharsetConvertStream.h"

using namespace std;

void Test1() {
    CharsetConvert converter;
    converter.Init("UTF-32LE", "UTF-8");

    const char str[] = "中文";
    const char* p = str;
    size_t len = 6;

    wchar_t wstr[100];
    char* pwstr = (char*)&wstr[0];
    size_t wstrLen = sizeof(wstr);
    ConvertResult res = converter.Convert(&p, &len, &pwstr, &wstrLen);
}

class MyConvertStream : public CharsetConvertStream {
private:
    void OnConvertedChars(char* str, size_t len);
};

void MyConvertStream::OnConvertedChars(char* str, size_t len) {
    wstring ws((wchar_t*)str, len >> 1);
    wcout << ws << endl;
}

void Test2() {
    MyConvertStream stream;
    stream.Init("UTF-32LE", "UTF-8");

    const char str[] = "中文";

    stream.Process(str, 1);
    stream.Process(str + 1, 1);
    stream.Process(str + 2, 3);
    stream.Process(str + 5, 1);
}

int main() {
    Test2();
    return 0;
}

