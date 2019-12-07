#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdbool.h>
#include <unistd.h>

#define READER_COUNT 10

#define LOG(x) \
if (x) fprintf(stderr, "%s error\n", #x)

char buffer[20];
pthread_cond_t conditional;
pthread_mutex_t mux;
int msg_id = 0;
int readers_left = 0;
bool writer_ended = false;


void* reader(void* arg) {
    // Czekaj, aż nie zostanie zapisana pierwsza wiadomość
    LOG (pthread_mutex_lock(&mux));
    while (msg_id == 0)
        LOG (pthread_cond_wait(&conditional, &mux));
    LOG (pthread_mutex_unlock(&mux));
    
    while (!writer_ended) {
        int current_msg = msg_id;
        printf("Przeczytano: %s\n", buffer);

        LOG (pthread_mutex_lock(&mux));
        readers_left--;
        LOG (pthread_cond_broadcast(&conditional));
        // czekaj na nową wiadomość lub koniec wiadomości
        while (msg_id == current_msg && !writer_ended)
            LOG (pthread_cond_wait(&conditional, &mux));
        LOG (pthread_mutex_unlock(&mux));
    }
    return NULL;
}

void* writer(void* arg) {
    int i;
    for (i = 0; i < 20; i++) {
        LOG (pthread_mutex_lock(&mux));
        sprintf(buffer, "wiadomosc #%d", i);
        printf("zapisano: %d\n", i);
        msg_id++;
        readers_left = READER_COUNT;
        LOG (pthread_cond_broadcast(&conditional));
        // czekaj aż każdy czytelnik przeczyta
        while (readers_left)
            LOG (pthread_cond_wait(&conditional, &mux));
        LOG (pthread_mutex_unlock(&mux));
    }
    // przekaż czytelnikom, że skończyłeś
    writer_ended = true;
    LOG (pthread_cond_broadcast(&conditional));
    return NULL;
}

int main(void) {
    LOG (pthread_cond_init(&conditional, NULL));
    LOG (pthread_mutex_init(&mux, NULL));

    pthread_t writer_thread, reader_threads[READER_COUNT];
    LOG (pthread_create(&writer_thread, 0, &writer, NULL));
    int i;
    for (i = 0; i < READER_COUNT; i++)
        LOG (pthread_create(reader_threads + i, 0, &reader, NULL));

    LOG (pthread_join(writer_thread, NULL));
    for (i = 0; i < READER_COUNT; i++)
        LOG (pthread_join(reader_threads[i], NULL));

    LOG (pthread_cond_destroy(&conditional));
    LOG (pthread_mutex_destroy(&mux));
}

