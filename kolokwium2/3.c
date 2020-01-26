#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// include dla FIFO -> są w "man 3 mkfifo"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

/**
 * Napisz program, w którym utworzone zostaną 3 wątki: główny i 2 pomocnicze.
 * Wątek główny powinien wysyłać do pozostałych 20 losowych liczb z przedziału
 * 64-256 co 3 sekundy, natomiast wątki pomocnicze powinny pobierać odbierać te
 * liczby. Pierwszy wątek powinien sumować liczby i wyświetlać je na ekranie
 * monitora, drugi - mnożyć. Do komunikacji należy użyc kolejki FIFO (łącza
 * nazwanego). Do wstrzymania wątku należy użyć funkcji sleep()
 */

void* add_thread(void* arg) {
    int fd = *((int*) arg);
    long val = 0;
    int i;
    for (i = 0; i < 20; i++) {
        int tmp;
        if (read(fd, &tmp, sizeof tmp) < 0) perror("read");
        val += tmp;
        printf("suma: %ld\n", val);
        sleep(1); // Żeby jeden nie odebrał 2 razy
    }
    return NULL;
}

void* mul_thread(void* arg) {
    int fd = *((int*) arg);
    long val = 1;
    int i;
    for (i = 0; i < 20; i++) {
        int tmp;
        if (read(fd, &tmp, sizeof tmp) < 0) perror("read");
        val *= tmp;
        printf("iloczyn: %ld\n", val);
        sleep(1); // Żeby jeden nie odebrał 2 razy
    }
    return NULL;
}

int main(void) {
    if (mkfifo("fifo", 0666) < 0) perror("mkfifo");
    puts("test");
    int fd = open("fifo", O_RDWR, 0666);
    puts("test");
    if (fd < 0) perror("open");

    pthread_t threads[2];
    if (pthread_create(&threads[0], NULL, &add_thread, (void*) &fd) < 0)
        puts("blad pthread_create");
    if (pthread_create(&threads[1], NULL, &mul_thread, (void*) &fd) < 0)
        puts("blad pthread_create");

    srand(time(NULL));
    int i;
    for (i = 0; i < 20; i++) {
        int val = (rand() + 64) % 257;
        printf("przesyłam: %d\n", val);
        if (write(fd, &val, sizeof val) < 0) perror("write");
        if (write(fd, &val, sizeof val) < 0) perror("write");
        // Zapis 2 razy bo jak jeden obierze to kasuje też z FIFO
        sleep(3); 
    }

    for (i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) < 0)
            puts("blad pthread_join");
    }

    if (close(fd) < 0) perror("close");
    if (unlink("fifo") < 0) perror("unlink");
    return 0;
}