#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/ssl.h>

void ErrorExit(const char* msg) {
    printf("\n%s(%d): %s\n", msg, errno, strerror(errno));
    exit(1);
}

void ErrorExit0(const char* msg) {
    printf("\nERROR: %s\n", msg);
    exit(1);
}

int SocketConnect(const char* host, short port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_aton(host, &addr.sin_addr);
    addr.sin_port = htons(port);
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        printf("Socket connected\n");
    } else {
        close(s);
        ErrorExit("Socket connect failed");
    }
    return s;
}

void MakeNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        ErrorExit("fcntl1");
    }

    flags |= O_NONBLOCK;

    int ret = fcntl(fd, F_SETFL, flags);
    if (ret == -1) {
        ErrorExit("fcntl2");
    }
}

void DoPoll(struct pollfd *fds, nfds_t nfds) {
    for (;;) {
        int ret = poll(fds, nfds, -1);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                ErrorExit("Poll failed");
            }
        } else if (ret == 0) {
            continue;
        } else {
            break;
        }
    }
}

int DoPollTimeOut(struct pollfd *fds, nfds_t nfds, int timeout) {
    for (;;) {
        int ret = poll(fds, nfds, timeout);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                ErrorExit("Poll failed");
            }
        } else if (ret == 0) {
            return 1;
        } else {
            break;
        }
    }
    return 0;
}

void MY_SSL_Connect(int sockFD, SSL* ssl) {
    for (;;) {
        int ret = SSL_connect(ssl);
        if (ret == 1)   // Success
            break;

        struct pollfd pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd = sockFD;

        int err = SSL_get_error(ssl, ret);
        if (err == SSL_ERROR_WANT_READ) {
            pfd.events = POLLIN;
        } else if (err == SSL_ERROR_WANT_WRITE) {
            pfd.events = POLLOUT;
        } else {
            ErrorExit0("SSL_connect");
        }

        DoPoll(&pfd, 1);
    }
}

int MY_SSL_ReadData(int sockFD, SSL* ssl, char* buf, int len) {
    for (;;) {
        int ret = SSL_read(ssl, buf, len);
        if (ret > 0)    // Success
            return ret;

        struct pollfd pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd = sockFD; 
        int err = SSL_get_error(ssl, ret);
        if (err == SSL_ERROR_WANT_READ) {
            pfd.events = POLLIN;
        } else if (err == SSL_ERROR_WANT_WRITE) {
            pfd.events = POLLOUT;
        } else {
            ErrorExit0("SSL_read");
        }

        DoPoll(&pfd, 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s [host] [port]\n", argv[0]);
        return 0;
    }
    char* host = argv[1];
    short port = (short)atoi(argv[2]);

    int s = SocketConnect(host, port);
    MakeNonBlocking(s);

    SSL_library_init();

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    //SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, s);

    MY_SSL_Connect(s, ssl);
    printf("SSL connected\n");

    X509* peerCert = SSL_get_peer_certificate(ssl);
    if (peerCert) {
        char text[256];
        X509_NAME* subjectName = X509_get_subject_name(peerCert);
        X509_NAME_get_text_by_NID(subjectName, NID_commonName, text, sizeof(text));
        printf("Common Name: %s\n", text);

        X509_free(peerCert);
    }

    for (;;) {
        char buf[1024];
        int n = MY_SSL_ReadData(s, ssl, buf, 1023);
        buf[n] = '\0';
        printf("Receive Data: %s\n", buf);
    }
    return 0;
}

