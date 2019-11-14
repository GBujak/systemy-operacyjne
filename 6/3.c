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

void worker(int shmid, int semset_id, int worker_id) {
    shared_t* shared = shmat(shmid, NULL, 0);
    if (shmid < 0) {
        perror("shmget");
        abort();
    }

    int i;
    for (i = 0; i < 10; i++) {
        struct sembuf operation = {0, (worker_id) ? -1 : 0, 0};
        if (semop(semset_id, &operation, 1) < 0)
            perror("semop");

        if (worker_id) {
            sprintf(shared->data, "Hello world - %d\n", i);
        } else {
            write(STDOUT_FILENO, shared->data, strlen(shared->data));
        }

        operation.sem_op = (worker_id) ? -1 : 2;
        if (semop(semset_id, &operation, 1) < 0)
            perror("semop");
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

    int semset_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semset_id < 0) perror("semget");

    // Inicjalizacja struktury
    shared_t* shared = shmat(shmid, NULL, 0);
    shared->access = 0;
    strcpy(shared->data, "Hello world\n");
    if (shmdt(shared) < 0) perror("shmdt");
    
    PID = fork();
    if (PID < 0) {
        perror("fork");
        abort();
    }
    if (PID == 0) worker(shmid, semset_id, 0);
    worker(shmid, semset_id, 1);

    if (wait(0) < 0) perror("wait");

    if (shmctl(shmid, IPC_RMID, NULL) < 0) perror("shmctl");

    if (semctl(semset_id, 0, IPC_RMID) < 0)
        perror("semctl");
    return 0;
}