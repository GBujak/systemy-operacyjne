#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

// Program wysyłający wiadomość

typedef struct {
    long type;
    char mtext[50];
} buffer_t;

typedef struct {
    long type;
    char mtext[500];
} buffer_long_t;

void send_message(int mqid, const char* message) {
    buffer_t buffer = {1, ""};
    memset(buffer.mtext, 0, sizeof(buffer.mtext));
    strncpy(buffer.mtext, message, 49);

    if (msgsnd(mqid, &buffer, sizeof(buffer.mtext), 0) < 0)
        perror("msgsnd");
}

void send_long_message(int mqid, const char* message) {
    buffer_long_t buffer = {2, ""};
    memset(buffer.mtext, 0, sizeof(buffer.mtext));
    strncpy(buffer.mtext, message, 499);

    if (msgsnd(mqid, &buffer, sizeof(buffer.mtext), 0) < 0)
        perror("msgsnd");
}

int main(void) {
    int key = ftok("/tmp", 8);
    if (key < 0) perror("ftok");
    int id = msgget(key, 0600);
    if (id < 0) perror("msgget");

    for (int i = 0; i < 5; i++) {
        if (i%2) {
            send_message(id, "long");
            send_long_message(id, "długa długa długa długa długa długa"
                                  "długa długa długa długa długa długa"
                                  "długa długa długa długa długa długa"
                                  "długa długa długa długa długa długa");
        } else {
            send_message(id, "short");
            send_message(id, "krótka krótka krótka");
        }
    }

    return 0;
}