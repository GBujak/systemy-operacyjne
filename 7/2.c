#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    int x, y;
} vec2d_t;

void* sum(void* arg) {
    vec2d_t* vec = (vec2d_t*) arg;
    printf("Funkcja sum: %d\n", vec->x + vec->y);
    return NULL;
}

void* diff(void* arg) {
    vec2d_t* vec = (vec2d_t*) arg;
    printf("Funkcja diff: %d\n", vec->x - vec->y);
    return NULL;
}

int main(void) {
    vec2d_t vec = { 16, 8 };
    pthread_t threads[2];
    
    pthread_create(threads + 0, NULL, &sum,  &vec);
    pthread_create(threads + 1, NULL, &diff, &vec);

    int i;
    for (i=0;i<2;i++) pthread_join(threads[i], NULL);
}