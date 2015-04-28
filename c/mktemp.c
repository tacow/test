#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;
    for(i = 0; i < 10; ++i) {
        char tmpFilename[] = "tmp_XXXXXX";
        mktemp(tmpFilename);
        FILE* f = fopen(tmpFilename, "w");
        fprintf(f, "File name: %s\n", tmpFilename);
        fclose(f);
    }

    return 0;
}

