#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    long type;
    int msg
} msgbuf;

void send(int msg) {
    msgbuf buff = { 1, 256 };
    if (msgsnd(msg, &buff, sizeof buff.msg, 0) < 0)
        perror("msgsnd");    
}

int main(void) {
    int key = ftok("/tmp", 8);
    if (key < 0) perror("ftok");

    int msg = msgget(key, 0666);
    if (msg < 0) perror("msgget");

    send(msg);

    return 0;
}