#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char** argv) {
    int val = (argc == 1) ? 0 : 1;
    char message[100];
    int descriptor = open("fifo", O_WRONLY);
    if (descriptor < 0) perror("open error");

    for (;;) {
        sprintf(message, "%d\0", val);
        if (write(descriptor, message, strlen(message)) < 0)
            perror("write error");

        val += 2;
        message[0] = '\0';
    }
}
