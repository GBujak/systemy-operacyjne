#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h> 

int PID;

void do_work(int semset_id) {
    if (PID == 0) {
        int i;
        for (i = 0; i < 50; i++) {
            struct sembuf operation = {0, 0, 0};
            if (semop(semset_id, &operation, 1) < 0)
                perror("semop");

            puts("potomek");

            operation.sem_op = 3;
            if (semop(semset_id, &operation, 1) < 0)
                perror("semop");
        }
    } else {
        int i;
        for (i = 0; i < 50; i++) {
            struct sembuf operation = {0, -2, 0};
            if (semop(semset_id, &operation, 1) < 0)
                perror("semop");

            puts("rodzic");

            operation.sem_op = -1;
            if (semop(semset_id, &operation, 1) < 0)
                perror("semop");
        }
    }

    if (PID == 0) exit(0);
    else if (wait(0) < 0)
        perror("wait");
}

int main(void) {
    int semset_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT | IPC_EXCL);
    if (semset_id < 0) perror("semget");

    PID = fork();
    if (PID < 0) perror("fork");
    
    do_work(semset_id);

    if (semctl(semset_id, 0, IPC_RMID) < 0)
        perror("semctl");
    return 0;
}
