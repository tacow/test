#include <stdio.h>

struct BF {
    int a : 10;
    int b : 10;
    int c : 12;
};

struct BF2 {
    int a : 10;
    int b : 10;
    unsigned int c : 12;
    unsigned char d : 1;
    unsigned char e : 1;
    unsigned char f : 1;
    long long g : 50;
    unsigned long long h : 12;
};

int main(int argc, char* argv[]) {
    struct BF bf;
    struct BF2 bf2;

    bf.a = 100;
    bf.b = -200;
    bf.c = 300;
    printf("%zu\n", sizeof(struct BF));
    printf("%d %d %d\n", bf.a, bf.b, bf.c);

    bf2.a = 100;
    bf2.b = -200;
    bf2.c = 300;
    bf2.d = 1;
    bf2.e = 0;
    bf2.f = 1;
    bf2.g = -500;
    bf2.h = 500;
    printf("%zu\n", sizeof(struct BF2));
    printf("%d %d %d %d %d %d %lld %llu\n", bf2.a, bf2.b, bf2.c, bf2.d, bf2.e, bf2.f, bf2.g, bf2.h);
    return 0;
}

