#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/file.h>

#define BUFSIZE 4096
char buffer[BUFSIZE];

long get_size(char* fname) {
    struct stat st;
    if (stat(fname, &st) < 0) perror("stat");
    return st.st_size;
}

void child_porn(char* from, char* to) {
    int from_desc = open(from, O_RDONLY, 0600);
    if (from_desc < 0) perror("open");
    int to_desc = creat(to, 0666);
    if (to_desc < 0) perror("open");

    long size = get_size(from);
    long left = size;

    while (left) {
        int readed = read(from_desc, buffer, BUFSIZE);
        write(to_desc, buffer, readed);
        left -= readed;
        printf("left = %ld\n", left);
    }

    close(from_desc);
    close(to_desc);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        puts("daj 2 pliki");
        return -1;
    }

    printf("kopiuje %s do %s\n", argv[1], argv[2]);
    child_porn(argv[1], argv[2]);

    return 0;
}