#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct {
    long type;
    char mtext[50];
} buffer_t;

void receive_message(int mqid, buffer_t* buffer) {
    if (msgrcv(mqid, buffer, sizeof(buffer->mtext), 1, 0) < 0)
        perror("msgrcv");
    else printf("otrzymano: %s\n", buffer->mtext);
}

void send_message(int mqid, buffer_t* buffer, const char* message) {
    buffer->type = 1;
    memset(buffer->mtext, 0, sizeof(buffer->mtext));
    strncpy(buffer->mtext, message, 49);

    if (msgsnd(mqid, buffer, sizeof(buffer->mtext), 0) < 0)
        perror("msgsnd");
}

int main(void) {
    int key = ftok("/tmp", 8);
    if (key < 0) perror("ftok");

    int id = msgget(key, 0600 | IPC_CREAT | IPC_EXCL);
    if (id < 0) perror("msgget");

    buffer_t buffer = {
        1, ""
    };
    int pid = fork();
    
    if (pid < 0) perror("fork");
    if (pid != 0) {
        receive_message(id, &buffer);
    } else {
        send_message(id, &buffer, "wiadomosc!");
        exit(0);
    }

    if (wait(0) < 0) perror("wait");

    if (msgctl(id, IPC_RMID, 0) < 0)
        perror("msgctl");

    return 0;
}
