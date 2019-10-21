#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 50
#define MESSAGE_TYPE 1

typedef struct {
    long type;
    char mtext[BUFFER_SIZE + 1]; // jeden znak więcej, żeby dodać w tym
} buffer_t;                    // miejscu znak końca ciągu znaków


void receive_msg(int mqid, buffer_t* buff) {
    if (msgrcv(mqid, buff, BUFFER_SIZE, MESSAGE_TYPE, 0) < 0) {
        perror("msgrcv");
    } else {
        buff->mtext[BUFFER_SIZE] = '\0';
        printf("Odebrano: %s\n", buff->mtext);
    }
}

void send_msg(int mqid, buffer_t* buff, const char* message) {
    strncpy(buff->mtext, message, BUFFER_SIZE);
    if (msgsnd(mqid, buff, BUFFER_SIZE, 0) < 0)
        perror("msgsnd");
}

int main(void) {
    // IPC_PRIVATE - kolejka dostępna tylko dla tego procesu lub procesów potomnych
    // IPC_CREAT - stwórz nową, jeśli nie istnieje
    // IPC_EXCL  - zwróć błąd, jeśli kolejka o tym kluczu już istnieje
    int id = msgget(IPC_PRIVATE, 0600 | IPC_CREAT | IPC_EXCL);
    if (id < 0) perror("msgget");

    buffer_t buff = { 1, "" };
    for (int i = 0; i < 5; i++) {
        send_msg(id, &buff, "!!! wiadomosc !!!");
        receive_msg(id, &buff);
    }

    // IPC_RMID - usuń kolejkę o danym ID
    if (msgctl(id, IPC_RMID, 0) < 0) perror("msgctl");
    return 0;
}