#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Test1() {
    char* buf = (char*)malloc(3);
    strcpy(buf, "abcdef");
    free(buf);
}

void Test2() {
    char* buf = new char[10];
    strcpy(buf, "abcdef");
    free(buf);
}

void Test3() {
    char* buf = new char[10];
    strcpy(buf, "abcdef");
    delete[] buf;
    delete[] buf;
}

void Test4() {
    char* buf = (char*)0x12345678;
    free(buf);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [case id](1,2,3,4)\n", argv[0]);
        return 0;
    }

    int caseID = atoi(argv[1]);
    switch (caseID) {
    case 1:
        Test1();
        break;
    case 2:
        Test2();
        break;
    case 3:
        Test3();
        break;
    case 4:
        Test4();
        break;
    }
    return 0;
}

