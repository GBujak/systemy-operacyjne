#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>

#define filename "test.txt"

void work(int argument) {
    int file = open(filename, O_RDWR, 0600);
    if (flock(file, LOCK_EX | LOCK_NB) < 0) {
        puts("blad w flock");
        return;
    }   
    sleep(1);
    puts("zamykam");
    close(file);
}

int main(void) {
    int pid = fork();
    if (pid < 0) perror("fork");

    work(420);

    if (pid == 0) exit(0);
    else if (wait(0) < 0) perror("wait");
}