#include "utils.h"
#include <pthread.h>

void* func(void* args) {
    int* client_sock = (int*) args;

    int msg;
    if (read(*client_sock, &msg, sizeof msg) < 0) {
        perror("read");
        return NULL;
    }
    printf("Odebrano: %d\n", msg);

    close(*client_sock);
    free(client_sock);
    return NULL;
}

int main(void) {
    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);
    bind_sock(sock, &addr);
    listen_sock(sock, 1);

    for (;;) {
        int* client_sock = malloc(sizeof(int));
        *client_sock = accept(sock, NULL, NULL);
        if (*client_sock < 0) {
            perror("accept");
            abort();
        }
        pthread_t thread = 0;
        if (pthread_create(&thread, NULL, &func, (void*) client_sock) < 0)
            puts("error pthread");
    }

    close(sock);
    return 0;
}