#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [command]\n", argv[0]);
        return 0;
    }

    char* command = argv[1];
    FILE* f = popen(command, "r");

    char line[1024];
    int  lineNum = 1;
    while(fgets(line, 1024, f)) {
        printf("%d: %s", lineNum, line);
        ++lineNum;
    }
    pclose(f);
    return 0;
}

