#include <unistd.h>
#include <string.h>
#include <termios.h>

int main(int argc, char* argv[]) {
    struct termios ti, ori_ti;
    memset(&ti, 0, sizeof(struct termios));
    tcgetattr(STDIN_FILENO, &ti);
    ori_ti = ti;
    ti.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    ti.c_cc[VMIN] = 1;
    ti.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ti);

    char buf[256];
    ssize_t n = 0;
    while (1) {
        n = read(STDIN_FILENO, buf, 256);
        if (n <= 0)
            break;
        if (n == 1 && buf[0] == '\e')
            break;
        write(STDOUT_FILENO, buf, n);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &ori_ti);
    return 0;
}

