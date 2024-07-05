#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define max(a,b)  (a>b?a:b)

typedef struct {
    long long _inp;
    long long c1_out;
    long long c2_out;
    long long _out;
} data_t;

data_t shared_data;
pthread_t TID[4];
pthread_attr_t attr;

long long fibo(long long x) {
    if (x==-2) return -1;
    else if (x==-1) return  1;
    else return fibo(x-1) + fibo(x-2);
}

int strToInt(const char* string) {
    int i=0, j=0, arg=0;
    while(string[i]) {
        j = string[i++];
        if (j > 47 && j < 58) {
            arg = 10 * arg + (j-48);
        } else {
            printf("Positive Integers Only !\n");
            exit(0);
        }
    }
    return arg;
}

void* getFibo1(void* arg) {
    // Child Thread-1
    data_t* tmp = (data_t*) arg;
    tmp->c1_out = fibo(tmp->_inp-2);
    pthread_exit(0);
}

void* getFibo2(void* arg) {
    // Child Thread-2
    data_t* tmp = (data_t*) arg;
    tmp->c2_out = fibo(tmp->_inp-1);
    pthread_exit(0);    
}

void* getSum(void* arg) {
	// Parent Thread
    data_t* tmp = (data_t*) arg;
	
    pthread_create(TID+0, &attr, getFibo1, (void*)&shared_data);
    pthread_create(TID+1, &attr, getFibo2, (void*)&shared_data);

    pthread_join(TID[0], NULL);
    pthread_join(TID[1], NULL);

    tmp->_out = tmp->c1_out + tmp->c2_out;

	pthread_exit(0);
}

void* printFib(void* arg) {
	// Print Thread
    data_t* tmp = (data_t*) arg;
	pthread_create(TID+2, &attr, getSum, (void*)&shared_data);
    pthread_join(TID[2], NULL);

    long long inp = tmp->_inp;
    long long out = tmp->_out;

    printf("%lldth Fibonacci number is:\t%lld\n", inp, out);
    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    // Get default thread attributes
    pthread_attr_init(&attr);

    if (argc == 2) {

        shared_data._inp = strToInt(argv[1]);
        pthread_create(TID+3, &attr, printFib,(void*) &shared_data);
    	pthread_join(TID[3], NULL);
    	pthread_exit(0);

    } else {
        puts("Exactly one argument needed !");
    }
    return 0;
}

