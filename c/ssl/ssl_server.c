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

int SocketAccept(short port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);
    addr.sin_port = htons(port);
    int s = socket(AF_INET, SOCK_STREAM, 0);

    int option = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        ErrorExit("bind");
    }
    listen(s, 10);
    int s1 = accept(s, NULL, NULL);
    if (s1 == -1) {
        ErrorExit("accept");
    }
    return s1;
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

const char* GetRandomStr() {
    static char g_strBuf[256];

    srand(time(NULL));
    int len = rand() % 30 + 20;
    
    int i;
    for (i = 0; i < len; ++len) {
        char randChar = 'a' + (rand() % 26);
        g_strBuf[i] = randChar;
    }
    g_strBuf[len] = '\0';
    return g_strBuf;
}

void MY_SSL_Accept(int sockFD, SSL* ssl) {
    for (;;) {
        int ret = SSL_accept(ssl);
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
            ErrorExit0("SSL_accept");
        }

        DoPoll(&pfd, 1);
    }
}

int MY_SSL_WriteData(int sockFD, SSL* ssl, const char* buf, int len) {
    for (;;) {
        int ret = SSL_write(ssl, buf, len);
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
            ErrorExit0("SSL_write");
        }

        DoPoll(&pfd, 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage: %s [port] [cert file] [privkey file]\n", argv[0]);
        return 0;
    }
    short port = (short)atoi(argv[1]);
    const char* certFile = argv[2];
    const char* privkeyFile = argv[3];

    int s = SocketAccept(port);
    MakeNonBlocking(s);

    SSL_library_init();

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    //SSL_CTX_set_min_proto_version(ctx, TLS1_2_VERSION);
    int ret = 0;
    ret = SSL_CTX_use_certificate_file(ctx, certFile, SSL_FILETYPE_PEM);
    if (ret != 1) {
        ErrorExit0("SSL_CTX_use_certificate_file");
    }
    ret = SSL_CTX_use_PrivateKey_file(ctx, privkeyFile, SSL_FILETYPE_PEM);
    if (ret != 1) {
        ErrorExit0("SSL_CTX_use_PrivateKey_file");
    }
    ret = SSL_CTX_check_private_key(ctx);
    if (ret != 1) {
        ErrorExit0("SSL_CTX_check_private_key");
    }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, s);

    MY_SSL_Accept(s, ssl);
    printf("SSL accepted\n");

    X509* peerCert = SSL_get_peer_certificate(ssl);
    if (peerCert) {
        char text[256];
        X509_NAME* subjectName = X509_get_subject_name(peerCert);
        X509_NAME_get_text_by_NID(subjectName, NID_commonName, text, sizeof(text));
        printf("Common Name: %s\n", text);

        X509_free(peerCert);
    }

    int i;
    for (i = 0; i < 10; ++i) {
        const char* str = GetRandomStr();
        int len = (int)strlen(str);
        while (len > 0) {
            int n = MY_SSL_WriteData(s, ssl, str, len);
            str += n;
            len -= n;
        }
    }
    SSL_free(ssl);
    close(s);
    return 0;
}

