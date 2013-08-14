#include <stdio.h>

int main(int argc, char* argv[], char* env[]) {
    int i = 0, j = 0;
    for(i = 0; i < argc; ++i) {
        printf("%d: %s\n", i, argv[i]);
    }
    printf("\n====================\n\n");
    while(env[j]) {
        printf("%d: %s\n", j, env[j]);
        ++j;
    }
    return 0;
}

