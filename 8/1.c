#include <termios.h>
#include <stdio.h>
#include <unistd.h>

int print_if_terminal(int f_desc) {
    if (isatty(f_desc)) {
        char* name = ttyname(f_desc);
        if (name == NULL) return -1;
        printf("nazwa: %s\n", name);
        return 0;
    } else return -1;
}

int main(void) {
    return print_if_terminal(STDIN_FILENO);
}
