#include "utils.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int make_sock() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        abort();
    }
    return sock;
}

struct sockaddr_in make_addr(uint16_t port) {
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    return addr;
}

// Klient
void connect_sock(int sock, struct sockaddr_in* addr) {
    int err = connect(sock, (struct sockaddr*) addr, sizeof *addr);
    if (err < 0) {
        perror("connect");
        abort();
    }
}

// Serwer
void bind_sock(int sock, struct sockaddr_in* addr) {
    int err = bind(sock, (struct sockaddr*) addr, sizeof *addr);
    if (err < 0) {
        perror("bind");
        abort();
    }
}

void listen_sock(int sock, int queue) {
    int err = listen(sock, queue);
    if (err < 0) {
        perror("listen");
        abort();
    }
}