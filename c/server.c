#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    short port;
    struct sockaddr_in addr;
    int s;
    int option = 1;
    if (argc < 2) {
        printf("Usage: %s [port]\n", argv[0]);
        return 0;
    }
    port = (short)atoi(argv[1]);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(port);
    s = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("bind failed(%d, %s)\n", errno, strerror(errno));
        close(s);
        return 0;
    }
    listen(s, 10);
    int s1 = accept(s, NULL, NULL);
    if (s1 != -1) {
        printf("success\n");
        close(s1);
    } else {
        printf("failed(%d, %s)\n", errno, strerror(errno));
    }
    close(s);
    return 0;
}

