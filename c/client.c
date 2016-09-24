#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>

void ErrorExit(const char* msg) {
  printf("\n%s(%d): %s\n", msg, errno, strerror(errno));
  exit(1);
}

void SendData(int s, const char* data, int len) {
  while(len) {
    struct pollfd pfd;
    pfd.fd = s;
    pfd.events = POLLOUT;
    pfd.revents = 0;
    int ret = poll(&pfd, 1, -1);
    if (ret < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        close(s);
        ErrorExit("Poll failed");
      }
    } else if (ret == 0) {
      continue;
    }

    if (pfd.revents & (POLLERR | POLLHUP)) {
      close(s);
      ErrorExit("Connection closed");
    }

    if (pfd.revents & POLLOUT) {
      ret = send(s, data, len, 0);
      if (ret < 0) {
        if (errno == EAGAIN) {
          continue;
        }

        close(s);
        ErrorExit("Send failed");
      } else {
        write(2, data, ret);
        data += ret;
        len -= ret;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  char* host;
  short port;
  int   s;
  struct sockaddr_in addr;
  if (argc < 3) {
    printf("Usage: %s [host] [port]\n", argv[0]);
    return 0;
  }
  host = argv[1];
  port = (short)atoi(argv[2]);
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  inet_aton(host, &addr.sin_addr);
  addr.sin_port = htons(port);
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0)
    printf("Connect succeed\n");
  else
    ErrorExit("Connect failed");

  char* data = "^ABCDEFGHIJKLMNOPQRSTUVWXYZ$";
  int len = strlen(data);
  while(1) {
    SendData(s, data, len);
  }
  return 0;
}

