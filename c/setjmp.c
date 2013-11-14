#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void banana() {
    printf("in banana()\n");
    longjmp(env, 1);
    printf("you'll never see this, because I longjmp'd\n");
}

int main() {
    if (setjmp(env) == 0) {
        printf("first time through\n");
        banana();
    } else {
        printf("back in main\n");
    }
    return 0;
}

