#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Max items produced by a producer
// Max items consumed by a consumer
#define MAXITEMS 5
#define BUFFSIZE 5
#define NUM_PROD 5
#define NUM_CONS 5

int item_ctr, buffer_in, buffer_out;
int buffer[BUFFSIZE];

pthread_t producers[NUM_PROD], consumers[NUM_CONS];
pthread_mutex_t mutex;
sem_t empty, filled;

void* producer(void* data) {
    for (int i = 0; i < MAXITEMS; i++)    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[buffer_in] = item_ctr;
        printf("Producer %d inserts Item %d at %d\n", *(int*)data, item_ctr, buffer_in);
        buffer_in = (buffer_in + 1) % BUFFSIZE;
        item_ctr++;
        pthread_mutex_unlock(&mutex);
        sem_post(&filled);
    }
}

void* consumer(void* data) {
    for (int i = 0; i < MAXITEMS; i++)    {
        sem_wait(&filled);
        pthread_mutex_lock(&mutex);
        printf("Consumer %d removes item %d from %d\n", *(int*)data, buffer[buffer_out], buffer_out);
        buffer_out = (buffer_out+1) % BUFFSIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char const *argv[])    {
    sem_init(&empty, 0, BUFFSIZE);
    sem_init(&filled, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    int i,j;
    int prod_index[NUM_PROD];
    int cons_index[NUM_CONS];

    for (i = 0; i < NUM_PROD; i++) {
        prod_index[i] = i+1;
        pthread_create(producers+i, NULL, (void*)producer, prod_index+i);
    }

    for (j = 0; j < NUM_CONS; j++) {
        cons_index[j] = j+1;
        pthread_create(consumers+j, NULL, (void*)consumer, cons_index+j);
    }

    for (i = 0; i < NUM_PROD; i++) {
        pthread_join(producers[i], NULL);
    }

    for (j = 0; j < NUM_CONS; j++) {
        pthread_join(consumers[j], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&filled);
    pthread_mutex_destroy(&mutex);

    return 0;
}
