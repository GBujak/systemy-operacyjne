#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

// Program odbierający wiadomość

typedef struct {
    long type;
    char mtext[50];
} buffer_t;

typedef struct {
    long type;
    char mtext[500];
} buffer_long_t;

void receive_message(int mqid) {
    buffer_t buffer = {1, ""};
    if (msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, 0) < 0)
        perror("msgrcv");
    else printf("otrzymano: %s\n", buffer.mtext);
}

void receive_long_message(int mqid) {
    buffer_long_t buffer = {2, ""};
    if (msgrcv(mqid, &buffer, sizeof(buffer.mtext), 2, 0) < 0)
        perror("msgrcv");
    else printf("otrzymano długą wiadomość: %s\n", buffer.mtext);
}

bool is_next_long(int mqid) {
    buffer_t buffer = {1, ""};
    if (msgrcv(mqid, &buffer, sizeof(buffer.mtext), 1, 0) < 0)
        perror("msgrcv");
    else {
        if (strcmp("long", buffer.mtext) == 0) 
             return true;
        else return false;
    }
    abort();
}

int main(void) {
    int key = ftok("/tmp", 8);
    if (key < 0) perror("ftok");
    int id = msgget(key, 0600 | IPC_CREAT | IPC_EXCL);
    if (id < 0) perror("msgget");

    for (int i = 0; i < 5; i++) {
        if (is_next_long(id))
             receive_long_message(id);
        else receive_message(id);
    }

    if (msgctl(id, IPC_RMID, 0) < 0)
        perror("msgctl");
    return 0;
}