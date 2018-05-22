#include "FastVolatile.h"
#include <stdio.h>

void Test1() {
    volatile long vl;
    for (long i = 0; i < 10000000000; ++i)
        vl = 100;
}

void Test2() {
    FastVolatile<long> fl;
    for (long i = 0; i < 10000000000; ++i)
        fl = 100;
}

void Test3() {
    long total = 0;
    FastVolatile<long> fl = 5;
    for (long i = 0; i < 1000; ++i) {
        if (fl) {
            total += fl;
            fl = fl + 1;
        }
    }
    printf("%ld\n", total);
}

int main() {
    Test3();
    return 0;
}

