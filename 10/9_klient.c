#include "utils.h"

#include <time.h>

int main(void) {
    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);

    connect_sock(sock, &addr);

    srand(time(NULL));
    int msg = rand() %101;
    if (write(sock, &msg, sizeof msg) < 0) {
        perror("write");
        abort();
    }

    close(sock);
    return 0;
}
