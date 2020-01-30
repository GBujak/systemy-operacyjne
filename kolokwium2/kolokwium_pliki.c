#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/wait.h>

int random_num() {
    return (rand() % 41) - 20;
}

void write_func(int fd) {
    if (flock(fd, LOCK_EX) < 0) perror("flock");
    int i;
    int num;
    for (i = 0; i < 10; i++) {
	num = random_num();
	printf("zapisuje: %d\n", num);
	if (write(fd, &num, sizeof(int)) < 0)
	    perror("write");
	if (fsync(fd) < 0) perror("fsync");
    }
    if (flock(fd, LOCK_UN) < 0) perror("flock");
}

void read_func(int fd) {
    if (flock(fd, LOCK_SH) < 0) perror("flock");
    int i;
    int num = 100;
    for (i = 0; i < 10; i++) {
	if (read(fd, &num, sizeof(int)) < 0)
	    perror("read");
	printf("Odczytano: %d\n", num);
    }
    if (flock(fd, LOCK_UN) < 0) perror("flock");
    if (close(fd) < 0) perror("close");
    exit(0);
}

int main(void) {
    srand(time(NULL));
    int fd = open("file.txt", O_RDWR | O_CREAT, 0666);

    int pid = fork();
    if (pid < 0) perror("fork");
    if (pid != 0) write_func(fd);
    else          read_func(fd);
    
    if (wait(0) < 0) perror("wait");

    if (close(fd) < 0) perror("close");
    return 0;
}
