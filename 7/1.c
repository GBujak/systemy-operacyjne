#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* func(void* arg) {
    printf(
        "PID: %d, thread ID: %lu\n", // pthread_self() zwraca long unsigned int
        getpid(), pthread_self()
    );
    return NULL;
}

int main(void) {
    int i;
    pthread_t threads[2];
    for (i=0;i<2;i++) pthread_create(threads + i, NULL, &func, NULL);
    for (i=0;i<2;i++) pthread_join(threads[i], NULL);
}