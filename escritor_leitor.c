#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t reader_t;
sem_t reader_sem;

pthread_t writer_t;
sem_t writer_sem;

void *reader_f();
void *writer_f();

int main() {
    sem_init(&reader_sem, 0, 1);
    sem_init(&writer_sem, 0, 1);

    for (int i = READER_CNT; i--) {
        pthread_create(&reader_t, NULL, reader_f, NULL);
    }


    return 0;
}
