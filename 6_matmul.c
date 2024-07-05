#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#define  SIZE 3

typedef struct {
    int mat1[SIZE][SIZE];
    int mat2[SIZE][SIZE];
    int matM[SIZE][SIZE];
} data_t;

data_t shared_data;
pthread_t arr[SIZE][SIZE];
pthread_attr_t attr;

int counter=-1, sum=0;

void* matMul(void* arg) {
    sum=0; counter++;
    // ptrs to arr of size SIZE
    int (* mat1)[SIZE] = ((data_t*)arg)->mat1;
    int (* mat2)[SIZE] = ((data_t*)arg)->mat2;

    for(int i=0; i<SIZE; i++) {
        sum += mat1[counter/SIZE][i] * mat2[i][counter%SIZE];
    }
    ((data_t*)arg)->matM[counter/SIZE][counter%SIZE]=sum;
    pthread_exit(0);
}

void printMAT(int (*mat)[SIZE]) {
    for (int i = 0; i < SIZE*SIZE; i++)    {
        if (! (i%SIZE)) puts("");
        printf("%d\t", mat[i/SIZE][i%SIZE]);
    }
    puts("\n");
}

int main(int argc, char const *argv[])    {
    // Get default thread attributes
    pthread_attr_init(&attr);
    // Random filling of values
    srand(time(NULL));
    for (int i = 0; i < SIZE*SIZE; i++)    {
        shared_data.mat1[i/SIZE][i%SIZE] = rand() % 11;
        shared_data.mat2[i/SIZE][i%SIZE] = rand() % 11;
    }
    // Print the matrices
    puts("Matrix 1:"); printMAT(shared_data.mat1);
    puts("Matrix 2:"); printMAT(shared_data.mat2);
    // Create SIZE * SIZE processes
    for (int i = 0; i < SIZE*SIZE; i++) {
        pthread_create (
            &arr[i/SIZE][i%SIZE], &attr,
            matMul, (void*) &shared_data
        );
    }
    // Print the resultant matrix
    for (int i = 0; i < SIZE*SIZE; i++)    {
        if (!i%SIZE) puts("");
        pthread_join(arr[i/SIZE][i%SIZE], NULL);
    }
    puts("Product:"); printMAT(shared_data.matM);
    return 0;
}
