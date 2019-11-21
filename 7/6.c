#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

void* func(void* arg) {
    sem_t* semaphore = (sem_t*) arg;
    sem_wait(semaphore);
    puts("wątek!");
    sem_post(semaphore);
    return NULL;
}

int main(void) {
    sem_t semaphore;
    unsigned int initial_value = 2;
    sem_init(&semaphore, 0, initial_value);

    pthread_t thread;
    pthread_create(&thread, NULL, &func, &semaphore);

    pthread_join(thread, NULL);

    sem_destroy(&semaphore);
}