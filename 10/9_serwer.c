#include "utils.h"

int main(void) {
    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);
    bind_sock(sock, &addr);
    listen_sock(sock, 5);

    puts("Odbieram 20 liczb");
    int i;
    for (i = 0; i < 20; ++i) {
        int client_sock = accept(sock, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            abort();
        }
        int msg;
        if (read(client_sock, &msg, sizeof msg) < 0) {
            perror("read");
            continue;
        }
        printf("Odebrano: %d\n", msg);
        close(client_sock);
    }

    close(sock);
    return 0;
}