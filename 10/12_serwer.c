#include "utils.h"
#include <sys/time.h>

long long get_miliseconds() {
    struct timeval te; 
    gettimeofday(&te, NULL);
    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
    return milliseconds;
}

int main(void) {
    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);
    bind_sock(sock, &addr);
    listen_sock(sock, 1);

    int client = accept(sock, NULL, NULL);
    if (client < 0) {
        perror("accept");
        abort();
    }

    size_t buffsize = 1024;
    char* buff = malloc(buffsize);
    
    long long ms = get_miliseconds();

    size_t received = 0;
    do {
        received = read(client, buff, buffsize);
        if (received < 0) {
            perror("read");
            abort();
        }
    } while (received);

    long long ms_after = get_miliseconds();

    close(client);
    free(buff);

    printf("Przesył zajął %lld ms\n", ms_after - ms);
}