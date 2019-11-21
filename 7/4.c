#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void* func(void* arg) {
    printf("wątek!\n");
    return NULL;
}

int main(void) {
    pthread_t joinable, detatched;

    pthread_attr_t attributes;
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);

    pthread_create(&joinable, NULL, &func, NULL);
    pthread_create(&detatched, &attributes, &func, NULL);

    printf(
        "Wynik funkcji pthread_join dla joinable:  %s\n",
        strerror(pthread_join(joinable, NULL))
    );
    printf(
        "Wynik funkcji pthread_join dla detatched: %s\n",
        strerror(pthread_join(detatched, NULL))
    );
}