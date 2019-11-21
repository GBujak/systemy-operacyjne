#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} vec2d_t;

void* sum(void* arg) {
    vec2d_t* vec = (vec2d_t*) arg;
    int* result = malloc(sizeof *result);
    *result = vec->x + vec->y;
    return result;
}

void* diff(void* arg) {
    vec2d_t* vec = (vec2d_t*) arg;
    int* result = malloc(sizeof *result);
    *result = vec->x - vec->y;
    return result;
}

int main(void) {
    vec2d_t vec = { 16, 8 };
    pthread_t threads[2];
    
    pthread_create(threads + 0, NULL, &sum,  &vec);
    pthread_create(threads + 1, NULL, &diff, &vec);

    int i;
    int* result;
    for (i=0;i<2;i++) {
        pthread_join(threads[i], (void*) &result);
        printf(
            "Funkcja %s zwróciła: %d\n",
            (i == 0) ? "sum " : "diff",
            *result
        );
        free(result);
    }
}