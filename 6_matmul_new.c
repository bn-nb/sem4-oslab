#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define  SIZE   200
#define  len(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
    int mat1[SIZE][SIZE];
    int mat2[SIZE][SIZE];
    int matM[SIZE][SIZE];
    int ctr;
} data_t;

data_t shared;
pthread_t arr[SIZE*SIZE];

double microsecs() {
    struct timeval now; gettimeofday(&now, NULL);
    return now.tv_sec + now.tv_usec / 1e6;
}

void* matMul(void* arg) {
    // ptrs to arr of size SIZE
    data_t* tmp = ((data_t*)arg);
    int sum=0;
    int ctr = (tmp->ctr);
    int (* mat1)[SIZE] = tmp->mat1;
    int (* mat2)[SIZE] = tmp->mat2;

    for(int i=0; i<SIZE; i++) {
        sum += mat1[ctr/SIZE][i] * mat2[i][ctr%SIZE];
    }
    tmp->matM[ctr/SIZE][ctr%SIZE]=sum;
    pthread_exit(0);
}

void thread_check(int m) {
    // m is a divisor of SIZE
    double time = microsecs();
    int q = SIZE*SIZE / m;

    memset(shared.matM, 0, SIZE*SIZE);
    pthread_attr_t attr; pthread_attr_init(&attr);
    shared.ctr = 0;

    for (int i = 0; i < q*m; i += m)    {
        for (int j = 0; j < m; j++, shared.ctr++)
            pthread_create(arr+j, &attr, matMul, (void*)&shared);

        for (int j = 0; j < m; j++)
            pthread_join(arr[j], NULL);
    }
    time = microsecs() - time;
    printf("%.6lf secs using %5d threads\n", time, m);
}

int main(int argc, char const *argv[])    {
    int chk[] = {1, 10, 40, 100, 400, 1000, 40000};
    printf("Multiply 2 square matrices of size %d\n", SIZE);
    for (int i = 0; i < len(chk); i++)    {
        thread_check(chk[i]);
    }
    return 0;
}
