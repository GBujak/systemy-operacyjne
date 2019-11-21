#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFSIZE 10
typedef struct {
    int start, end, lenght;
    int data[BUFSIZE];
} buffer_t;

pthread_cond_t updated;
pthread_mutex_t buffer_ownership;
buffer_t buffer;

void push_wait(buffer_t* buff, int val) {
    pthread_mutex_lock(&buffer_ownership);

    while (buffer.lenght == BUFSIZE) {
        puts("waiting on change in push");
        pthread_cond_wait(&updated, &buffer_ownership);
    }
    
    buffer.data[buffer.end] = val;
    buffer.end = (buffer.end + 1) % BUFSIZE;
    buffer.lenght++;

    pthread_mutex_unlock(&buffer_ownership);
    pthread_cond_broadcast(&updated);
}

int pop_wait(buffer_t* buff) {
    pthread_mutex_lock(&buffer_ownership);

    while (buffer.lenght == 0) {
        puts("waiting on change in pop");
        pthread_cond_wait(&updated, &buffer_ownership);
    }

    int val = buffer.data[buffer.start];
    buffer.start = (buffer.start + 1) % BUFSIZE;
    buffer.lenght--;

    pthread_mutex_unlock(&buffer_ownership);
    pthread_cond_broadcast(&updated);
    return val;
}

void* producer(void* arg) {
    int i;
    for (i = 0; i < 100; i++) {
        push_wait(&buffer, i);
    }
    return NULL;
}

void* consumer(void* arg) {
    int i;
    for(i = 0; i < 100; i++) {
        printf("read: %d\n", pop_wait(&buffer));
    }
    return NULL;
}

int main(void) {
    memset(&buffer, 0, sizeof buffer);
    pthread_mutex_init(&buffer_ownership, NULL);
    pthread_cond_init(&updated, NULL);

    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread, NULL, &producer, NULL);
    pthread_create(&consumer_thread, NULL, &consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
}