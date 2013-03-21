#include <stdio.h>

int add(int, int);
int sub(int, int);

int main() {
    int a = 5, b = 3;
    printf("%d %d\n", add(a, b), sub(a, b));
    return 0;
}

