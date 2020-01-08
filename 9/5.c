#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define FILENAME "pride.txt"

struct flock fl;

void read_file(int filedesc, int size) {
    fl.l_type  = F_RDLCK;
    fl.l_start = 0;
    fl.l_len   = size;

    usleep(100);

    if (fcntl(filedesc, F_SETLKW, &fl) < 0) {
        perror("fcntl");
        abort();
    }

    char buffer[51];
    if (lseek(filedesc, 0, SEEK_SET) < 0) perror("fseek");
    if (read(filedesc, buffer, 50) < 0) {
        perror("read");
        abort();
    }
    buffer[50] = '\0';

    printf(
        "===========================================\n"
        "Proces %d odczytał\n %s\n"
        "===========================================\n",
        getpid(), buffer
    );

    fl.l_type = F_UNLCK;
    if (fcntl(filedesc, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }
    if (close(filedesc) < 0) perror("close");

    exit(0);
}

void write_file(int filedesc, int size) {
    fl.l_type  = F_WRLCK;
    fl.l_start = 0;
    fl.l_len   = size;

    if (fcntl(filedesc, F_SETLKW, &fl) < 0) {
        perror("fcntl");
        abort();
    }

    char buffer[51];
    int i;
    for (i = 0; i < 51; i++) buffer[i] = 'X';

    if (write(filedesc, buffer, 50) < 0) {
        perror("write");
        abort();
    }

    fl.l_type = F_UNLCK;
    if (fcntl(filedesc, F_SETLK, &fl) == -1) {
        perror("fcntl");
        exit(1);
    }
    if (close(filedesc) < 0) perror("close");

    exit(0);
}

int main(void) {
    int filedesc = open(FILENAME, O_RDWR, 0600);
    int pid = 0;
    int i;
    for (i = 0; i < 4; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            abort();
        }

        if (pid == 0) {
            if (i == 0) write_file(filedesc, 50);
                   else read_file(filedesc, 50);
        }
    }

    for (i = 0; i < 4; i++)
        if (wait(0) < 0) perror("wait");

    if (close(filedesc) < 0) perror("close");

    return 0;
}
