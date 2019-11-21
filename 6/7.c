#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

int PID;

typedef struct {
    int access;
    char data[256];
} shared_t;

void worker(int shmid, int worker_id) {
    shared_t* shared = shmat(shmid, NULL, 0);
    if (shmid < 0) {
        perror("shmget");
        abort();
    }

    int i = 0;
    while (i < 10) {
        if (shared->access == worker_id) {
            printf("Proces %d: %s\n", worker_id, shared->data); // drukowanie wiadomości i numeru procesu
            shared->access = (shared->access + 1) % 3; // przełączenie dostępu na kolejny proces
            ++i; // inkrementacja "i" w celu zatrzymania pętli
        } else {
            usleep(100); // spanie 100 milisekund
        }
    }

    if (shmdt(shared) < 0) perror("shmdt");
    if (PID == 0) exit(0); // procesy potomne giną
}

int main(void) {
    int shmid = shmget(IPC_PRIVATE, sizeof(shared_t), 0600|IPC_CREAT|IPC_EXCL);
    if (shmid < 0) {
        perror("shmget");
        abort();
    }

    // Inicjalizacja struktury
    shared_t* shared = shmat(shmid, NULL, 0);
    shared->access = 0;
    strcpy(shared->data, "Hello world!");
    if (shmdt(shared) < 0) perror("shmdt");

    PID = fork();
    if (PID < 0) {
        perror("fork");
        abort();
    }
    if (PID == 0) worker(shmid, 0);
    PID = fork();
    if (PID < 0) {
        perror("fork");
        abort();
    }
    if (PID == 0) worker(shmid, 1);
    worker(shmid, 2);

    if (wait(0) < 0) perror("wait");
    if (wait(0) < 0) perror("wait");

    if (shmctl(shmid, IPC_RMID, NULL) < 0) perror("shmctl");
}