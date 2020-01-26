#include "utils.h"

int main(void) {
    int sock = make_sock();
    struct sockaddr_in addr = make_addr(8080);
    bind_sock(sock, &addr);
    listen_sock(sock, 5);

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);

    int maxfd = sock;

    int children_buff[50];
    int children_count = 0;

    for (;;) {
        int retval = select(maxfd+1, &fdset, NULL, NULL, NULL);
        if (retval < 0) {
            perror("select");
        } 
        if (FD_ISSET(sock, &fdset)) {
            int new = accept(sock, NULL, NULL);
            puts("dodano nowego klienta");
            if (new < 0) perror("accept");
            FD_SET(new, &fdset);
            if (new > maxfd) maxfd = new;
            children_buff[children_count++] = new;
        }
        int i;
        for (i=0; i<children_count; i++) {
            if (!FD_ISSET(children_buff[i], &fdset)) continue;
            int msg;
            if (read(children_buff[i], &msg, sizeof msg) < 0) {
                perror("read");
                continue;
            }
            printf("Odczytano %d\n", msg);
            FD_CLR(children_buff[i], &fdset);
            if (close(children_buff[i]) < 0)
                perror("close");
            int j;
            for (j = i; i < children_count - 1; j++)
                children_buff[j] = children_buff[j+1];
            children_count--;
            printf("usunięto klienta %d\n", i);
        }
    }

    if (close(sock) < 0)
        perror("close");
    return 0;
}
