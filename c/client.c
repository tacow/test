#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    char* host;
    short port;
    int   count = 1;
    int   i;
    int   s;
    struct sockaddr_in addr;
    if (argc < 3) {
        printf("Usage: %s [host] [port] [count]\n", argv[0]);
        return 0;
    }
    host = argv[1];
    port = (short)atoi(argv[2]);
    if (argc >= 4)
        count = atoi(argv[3]);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton(host, &addr.sin_addr);
    addr.sin_port = htons(port);
    for(i = 0; i < count; ++i) {
        printf("%d start\n", i);
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0)
            printf("success\n");
        else
            printf("failed(%d): %s\n", errno, strerror(errno));
        close(s);
    }
    return 0;
}

