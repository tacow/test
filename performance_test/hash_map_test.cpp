#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

#define TEST_N (10 * 1024 * 1024)
#define BUCKET_COUNT (20 * 1024 * 1024)

char RandChar() {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static const int n = sizeof(chars) - 1;
    return chars[rand() % n];
}

string RandStr() {
    int len = rand() % 10 + 5;
    string str;
    str.reserve(len);
    for (int i = 0; i < len; ++i) {
        char c = RandChar();
        str.append(1, c);
    }
    return str;
}

void HashMapTest() {
    srand(time(NULL));
    unordered_map<string, string> dict(BUCKET_COUNT);
    for (int i = 0; i < TEST_N; ++i) {
        string key = RandStr();
        string value = RandStr();
        dict[key] = value;
    }
}

void MapTest() {
    srand(time(NULL));
    map<string, string> dict;
    for (int i = 0; i < TEST_N; ++i) {
        string key = RandStr();
        string value = RandStr();
        dict[key] = value;
    }
}

int main() {
    time_t startTime = time(NULL);
    HashMapTest();
    time_t endTime = time(NULL);
    printf("unordered_map used %d seconds.\n", (int)(endTime - startTime));

    startTime = time(NULL);
    MapTest();
    endTime = time(NULL);
    printf("map used %d seconds.\n", (int)(endTime - startTime));

    return 0;
}

