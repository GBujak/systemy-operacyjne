#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define FLAG IPC_NOWAIT
// #define FLAG 0

typedef struct {
    long type;
    int msg
} msgbuf;

void receive(int msg) {
    msgbuf buff;
    if (msgrcv(msg, &buff, sizeof buff.msg, 1, FLAG) < 0) {
        perror("msgrcv");
        return;
    }
    printf("Otrzymano: %d\n", buff.msg);    
}

int main(void) {
    int key = ftok("/tmp", 8);
    if (key < 0) perror("ftok");

    int msg = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    if (msg < 0) perror("msgget");

    receive(msg);

    if (msgctl(msg, IPC_RMID, NULL) < 0) perror("msgctl");
    return 0;
}