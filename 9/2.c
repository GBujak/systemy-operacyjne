#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

void make_file(int filedesc, int size) {
    char buffer[11] = "XXXXXXXXX\n";

    int i;
    for (i = 0; i < size/2; i++)
        buffer[i] = 'X';

    if (write(filedesc, buffer, 10) < 0)
        perror("write");

    if (lseek(filedesc, size - 10 * 2, SEEK_CUR) < 0) perror("fseek");

    if (write(filedesc, buffer, 10) < 0)
        perror("write");
}

int main(void) {
    if (creat("hole.txt", 0600) < 0) perror("creat");
    int filedesc = open("hole.txt", O_RDWR, 0600);
    if (filedesc < 0) perror("open");

    make_file(filedesc, 512);
    
    if (close(filedesc) < 0) perror("close");

    return 0;
}
