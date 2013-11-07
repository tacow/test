#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int s;
    struct sockaddr_in addr;
    unsigned short port;

    for(port = 1; port < 65535; ++port) {
        s = socket(AF_INET, SOCK_STREAM, 0);
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htons(INADDR_ANY);
        addr.sin_port = htons(port);
        if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
            printf("%d: bind failed(%d, %s)\n", port, errno, strerror(errno));
        }
        close(s);
    }
    return 0;
}

