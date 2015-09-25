#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [size]\n", argv[0]);
        return 0;
    }
    size_t size = strtoul(argv[1], NULL, 0);

    void* p = malloc(size);
    printf("%p\n", p);
    free(p);
    return 0;
}

