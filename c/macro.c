#include <stdio.h>

#define OK1
#define OK2

int main() {
#if defined(OK1) && defined(OK2)
    printf("Ok\n");
#endif
}

