#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/**
 * Polecenie:
 * Stwórz dwa procesy. Jeden ma wysłać do drugiego 20 liczb w przedziale -10 - 10
 * a drugi ma je wyświetlić na ekranie
 */

typedef struct {
    long type;
    int msg;
} msg_t;

int random_number() {
    return rand() % 21 - 10;
}

void send_letters(int msgid) {
    msg_t buffer = { 1, 0 };
    int i;

    for (i = 0; i < 20; i++) {
        buffer.msg = random_number();
        if (msgsnd(msgid, &buffer, sizeof buffer.msg, 0) < 0)
            perror("msgsnd");
    }
}

void receive_letters(int msgid) {
    msg_t buffer = { 1, 0 };
    int i;

    for (i = 0; i < 20; i++) {
        if (msgrcv(msgid, &buffer, sizeof buffer.msg, 1, 0) < 0)
            perror("msgrcv");
        printf("Odebrano: %3d\n", buffer.msg);
    }
}

int main(void) {
    srand(time(NULL));

    int msgid = msgget(IPC_PRIVATE, 0600 | IPC_CREAT | IPC_EXCL);

    int pid = fork();
    if (pid < 0) {
        perror("fork");
        abort();
    } else if (pid == 0) {
        send_letters(msgid);
        exit(0);
    } else {
        receive_letters(msgid);
    }

    if (wait(0) < 0) perror("wait");
    if (msgctl(msgid, IPC_RMID, 0) < 0)
        perror("msgctl");

    return 0;
}
