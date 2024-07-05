#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_PHIL 50

pthread_t philosophers[NUM_PHIL];
sem_t chopsticks[NUM_PHIL];
int phil_index[NUM_PHIL];

void helper(int n) {
    int l = n;
    int r = (n+1) % NUM_PHIL;

    printf("Philosopher %d is thinking...\n", n);
    sleep(1);

    // Pickup chopsticks and eat
    if (n == 0) { // To remove deadlock
        sem_wait(chopsticks+r);
        printf("Philosopher %d has taken chopstick %d\n", n, r);
        sem_wait(chopsticks+l);
        printf("Philosopher %d has taken chopstick %d\n", n, l);
    } else {
        sem_wait(chopsticks+l);
        printf("Philosopher %d has taken chopstick %d\n", n, l);
        sem_wait(chopsticks+r);
        printf("Philosopher %d has taken chopstick %d\n", n, r);
    }
    printf("Philosopher %d is eating...\n", n);
    sleep(1);
    
    
    // Release chopsticks
    printf("Philosopher %d releases chopsticks\n", n);
    sem_post(chopsticks+r);
    sem_post(chopsticks+l);
}

void* philosopher(void* data) {
    int n = *(int*) data;
    while (1) {
        helper(n);
    }
}

int main(int argc, char const *argv[])    {
    
    for (int i = 0; i < NUM_PHIL; i++)    {
        sem_init(chopsticks+i, 0, 1);
    }

    for (int i = 0; i < NUM_PHIL; i++)    {
        phil_index[i] = i;
        pthread_create(philosophers+i, NULL, philosopher, phil_index+i);
    }

    for (int i = 0; i < NUM_PHIL; i++)    {
        pthread_join(philosophers[i], NULL);
    }
    return 0;
}
