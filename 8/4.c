#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct termios term_settings;

void switch_uppercase(struct termios* termset) {
    termset->c_oflag ^= OLCUC;
}

int main(void) {
    if (tcgetattr(STDIN_FILENO, &term_settings) < 0)
        fprintf(stderr, "błąd tcgetattr\n");

    switch_uppercase(&term_settings);

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term_settings) < 0)
        fprintf(stderr, "błąd tcsetattr old\n");
    return 0;
}