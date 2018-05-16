// @Copyrigth 2018 Rodrigo Moraes, Wesley Rocha

#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UP(x) sem_post(&x)
#define DOWN(x) sem_wait(&x)

#define BUFFER_DEFAULT '_'
#define BUFFER_MAX 8

#define PRODUCER_CNT 2

#define CONSUMER_CNT 2

#define TIME_INCREMENT 0.2 - M_PI / 2.0

pthread_t producer_t, consumer_t;
sem_t mutex, empty, filled;
char buffer[BUFFER_MAX];
char to_produce[] = "0123456789ABCDEF";
int empty_cnt = BUFFER_MAX;
int filled_cnt = 0;
int to_produce_cnt = 0;
int timer = 0;

void *produce();
void *consume();
void bufferInit();
void bufferProduce();
void bufferConsume();
void bufferDisplay();

int main() {
  srand(time(NULL));

  bufferInit();

  sem_init(&mutex, 0, 1);
  sem_init(&empty, 0, BUFFER_MAX);
  sem_init(&filled, 0, 0);

  for (int i = PRODUCER_CNT; i--;) {
    pthread_create(&producer_t, NULL, produce, NULL);
  }

  for (int i = CONSUMER_CNT; i--;) {
    pthread_create(&consumer_t, NULL, consume, NULL);
  }

  for (int i = PRODUCER_CNT; i--;) {
    pthread_join(producer_t, NULL);
  }

  for (int i = CONSUMER_CNT; i--;) {
    pthread_join(consumer_t, NULL);
  }

  return 0;
}

void *produce() {
  while (to_produce[to_produce_cnt] != '\0') {
    DOWN(empty);
    DOWN(mutex);
    bufferProduce();
    UP(filled);
    UP(mutex);

    sleep(1 + sin(timer));
    timer += TIME_INCREMENT;
  }
}

void *consume() {
  while (filled_cnt > 0 || to_produce[to_produce_cnt] != '\0') {
    DOWN(filled);
    DOWN(mutex);
    bufferConsume();
    UP(empty);
    UP(mutex);

    sleep(1 + cos(timer));
    timer += TIME_INCREMENT;
  }
}

void bufferInit() {
  for (int i = 0; i < BUFFER_MAX; i++) {
    buffer[i] = BUFFER_DEFAULT;
  }
}

void bufferProduce() {
  if (empty_cnt > 0) {
    buffer[filled_cnt] = to_produce[to_produce_cnt];
    empty_cnt--;
    filled_cnt++;
    to_produce_cnt++;
    bufferDisplay();
  }
}

void bufferConsume() {
  if (filled_cnt > 0) {
    buffer[BUFFER_MAX - empty_cnt - 1] = BUFFER_DEFAULT;
    filled_cnt--;
    empty_cnt++;
    bufferDisplay();
  }
}

void bufferDisplay() {
  for (int i = 0; i < BUFFER_MAX; i++) {
    printf("| %c ", buffer[i]);
  }
  printf("|\n");
}
