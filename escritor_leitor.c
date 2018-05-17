#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define UP(x) sem_post(&x)
#define DOWN(x) sem_wait(&x)

#define READER_CNT 2
#define WRITER_CNT 2

#define READER_MAX 10
#define WRITER_MAX 5

int reader_cnt = 0;

pthread_t reader_t;
pthread_t writer_t;

sem_t reader_sem;
sem_t writer_sem;

void *reader_f();
void *writer_f();

int global_thread_id = 0;

int main() {
    sem_init(&reader_sem, 0, 1);
    sem_init(&writer_sem, 0, 1);

    for (int i = READER_CNT; i--;) {
        pthread_create(&reader_t, NULL, reader_f, NULL);
    }

    for (int i = WRITER_CNT; i--;) {
        pthread_create(&writer_t, NULL, writer_f, NULL);
    }

    for (int i = READER_CNT; i--;) {
        pthread_join(reader_t, NULL);
    }
    
    for (int i = WRITER_CNT; i--;) {
        pthread_join(writer_t, NULL);
    }

    return 0;
}

void *reader_f() {
    int thread_id = global_thread_id++;
    for (int reader_id = 0; reader_id < READER_MAX; reader_id++) {
        DOWN(reader_sem);
        if (++reader_cnt == 1) {
            DOWN(writer_sem);
        }
        UP(reader_sem);
        printf("Thread: %d | Iteration: %d | Reading...\n", thread_id, reader_id);
        DOWN(reader_sem);
        if (--reader_cnt == 0) {
            UP(writer_sem);
        }
        UP(reader_sem);
        sleep(rand()%3);
    }
}

void *writer_f() {
    int thread_id = global_thread_id++;
    for (int writer_id = 0; writer_id < WRITER_MAX; writer_id++) {
        DOWN(writer_sem);
        printf("Thread: %d | Iteration: %d | Writing...\n", thread_id, writer_id);
        UP(writer_sem);
        sleep(rand()%3);
    }
}
