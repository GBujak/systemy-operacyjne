#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#define READER_COUNT 5

#define LOG(x) \
if (x) fprintf(stderr, "%s error\n", #x)

char buffer[20];
pthread_cond_t conditional;
sem_t semaphore;
pthread_mutex_t mut;
// mutex blokuje semafor - czytelnicy czekają na odblokowanie
bool writing = false;
bool writer_ended = false;

void* reader(void* arg) {
    while (!writer_ended) {
        LOG (pthread_mutex_lock(&mut));
        while (writing)
            pthread_cond_wait(&conditional, &mut);
        LOG (sem_wait(&semaphore));
        LOG (pthread_mutex_unlock(&mut));
        
        usleep(1000);
        printf("Czytelnik odczytał: %s\n", buffer);

        LOG (pthread_mutex_lock(&mut));
        LOG (sem_post(&semaphore));
        LOG (pthread_cond_broadcast(&conditional));
        LOG (pthread_mutex_unlock(&mut));
    }
    return NULL;
}

void* writer(void* arg) {
    int i;
    for (i = 0; i < 20; i++) {
        usleep(1000);
        LOG (pthread_mutex_lock(&mut));
        int readers = 0;
        LOG (sem_getvalue(&semaphore, &readers));
        while (writing || readers != READER_COUNT) {
            LOG (pthread_cond_wait(&conditional, &mut));
            LOG (sem_getvalue(&semaphore, &readers));
        }
        writing = true;

        sprintf(buffer, "Zapis %d", i);

        writing = false;
        LOG (pthread_cond_signal(&conditional));
        LOG (pthread_mutex_unlock(&mut));
    }
    LOG (pthread_mutex_lock(&mut));
    writer_ended = true;
    LOG (pthread_mutex_unlock(&mut));

    return NULL;
}

int main(void) {
    LOG (pthread_cond_init(&conditional, NULL));
    LOG (sem_init(&semaphore, 0, READER_COUNT));
    LOG (pthread_mutex_init(&mut, NULL));

    pthread_t writer_thread, reader_threads[READER_COUNT];
    LOG (pthread_create(&writer_thread, 0, &writer, NULL));
    int i;
    for (i = 0; i < READER_COUNT; i++)
        LOG (pthread_create(reader_threads + i, 0, &reader, NULL));

    LOG (pthread_join(writer_thread, NULL));
    for (i = 0; i < READER_COUNT; i++)
        LOG (pthread_join(reader_threads[i], NULL));

    LOG (pthread_cond_destroy(&conditional));
    LOG (sem_destroy(&semaphore));
    LOG (pthread_mutex_destroy(&mut));
}

