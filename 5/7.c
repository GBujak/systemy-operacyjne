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
        struct sembuf operation = {0, 1, SEM_UNDO};
        if (semop(semset_id, &operation, 1) < 0)
            perror("semop");
        puts("Dodano 1 do semafora!");
        sleep(2);
        exit(0);
    } else {
        sleep(1);
        struct sembuf operation = {0, 0, 0};
        if (semop(semset_id, &operation, 1) < 0)
            perror("semop");
        puts("Semafor miał wartość 0");
        if (wait(0) < 0)
            perror("wait");
    }
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
