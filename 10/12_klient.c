#include "utils.h"
#include <sys/time.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        puts("Podaj rozmiar pakietu w bajtach");
        return 0;
    }
    size_t size = 0;
    sscanf(argv[1], "%lu", &size);
    printf("Mierzenie przesłania %lu bajtów", size);

    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);

    char* data = calloc(size, sizeof(char));
    connect_sock(sock, &addr);
    size_t left = size;
    while (left) {
        printf("left: %lu\n", left);
        left -= write(sock, data, left);
        if (left < 0) {
            perror("write");
            abort();
        }
    }
    close(sock);
}