#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(void) {
    if (mkfifo("fifo", 0600) < 0)
        perror("mkfifo error");
    int descriptor = open("fifo", O_RDONLY);
    if (descriptor < 0) perror("descriptor error");

    int tmp, sum = 0;
    char message[100] = "";
    for(;;) {
        if (read(descriptor, message, sizeof(message)) < 0)
            perror("reading error");
        sscanf(message, "%d", &tmp);
        printf("Received\t%d\n", tmp);
        sum += tmp;
        printf("Current sum\t%d", sum);
    }
}
