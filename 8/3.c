#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct termios old_settings;
struct termios term_settings;

void set_noncanon(struct termios* termset) {
    // skopiowane z internetu ↙
    termset->c_lflag ^= ICANON;
    termset->c_lflag &= ~(ICANON|ECHO);
    termset->c_cc[VMIN] = 1;
    termset->c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, termset) < 0)
        fprintf(stderr, "błąd set noncanon\n");
}

void restore_settings(struct termios* termset) {
    if (tcsetattr(STDIN_FILENO, TCSANOW, termset) < 0)
        fprintf(stderr, "błąd restore\n");
}

const char* menu_loop(const char** options, int option_count) {
    set_noncanon(&term_settings);
    int selection = 0;
    char input;
    do {
        system("clear");
        int i;
        for (i = 0; i < option_count; i++) {
            printf(
                (i == selection) ? " ->  %s\n" : "     %s\n",
                options[i]
            );
        }
        input = getc(stdin);
        switch (input) {
            case 'j':
                selection = (selection + 1) % option_count;
                break;
            case 'k':
                selection = (selection + option_count - 1) % option_count;
                break;
        }
    } while (input != '\n');
    restore_settings(&old_settings);
    return options[selection];
}

int main(void) {
    if (tcgetattr(STDIN_FILENO, &term_settings) < 0)
        fprintf(stderr, "błąd tcgetattr\n");
    memcpy(&old_settings, &term_settings, sizeof term_settings);

    const char* options[] = {
        "opcja 1", "opcja 2", "opcja 3", "opcja 4",
        "opcja 5", "opcja 6", "opcja 7", "opcja 8"
    };
    const char* selection = menu_loop(options, 8);
    printf("wybór: %s\n", selection);

    return 0;
}