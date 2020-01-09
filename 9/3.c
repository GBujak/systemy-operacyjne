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
#define filename_2 "test2.txt"

void demonstracja_1(const char* fname) {
    int fd_1 = open(fname, O_WRONLY | O_CREAT, 0666);
    int fd_2 = dup(fd_1);
    close(fd_1);
    write(fd_2, "TEST\n", 5);
    // Zamknięcie fd_1 nie spowodowało zamknięcia fd_2
}

void demonstracja_2(const char* fname) {
    int fd = open(fname, O_WRONLY | O_CREAT, 0666);
    close(STDOUT_FILENO);
    dup(fd);
    close(fd);
    printf("Tekst powinien zostać zapisany do pliku\n");
    // funkcja dup używa jako nowego deskryptora najmniejszej liczby, która nie
    // jest już deskryptorem innego pliku - w tym przypadku 1 - liczba, króra
    // zazwyczaj oznacza standartowe wyjście i której używa funkcja printf
}

int main(void) {
    demonstracja_1(filename);
    demonstracja_2(filename_2);
    return 0;
}