#include <stdio.h>
#include <string.h>
#include <deque>
#include "timer.h"

using namespace std;

#define TEST_COUNT 10000000
#define OBJECT_SIZE 256

struct St {
    char arr[OBJECT_SIZE];
};

void Test1() {
    deque<St> dq;
    for(int i = 0; i < TEST_COUNT; ++i) {
        St st;
        dq.push_back(st);
    }
}

void Test2() {
    deque<St*> dq;
    for(int i = 0; i < TEST_COUNT; ++i) {
        St* st = new St();
        dq.push_back(st);
    }
    for(deque<St*>::iterator it = dq.begin(); it != dq.end(); ++it) {
        St* st = *it;
        delete st;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [1 / 2]\n", argv[0]);
        return 0;
    }

    Timer timer;
    timer.Start();

    if (strcmp(argv[1], "1") == 0)
        Test1();
    if (strcmp(argv[1], "2") == 0)
        Test2();

    timer.Stop();
    printf("Microseconds: %ld\n", timer.GetMicroseconds());

    return 0;
}

