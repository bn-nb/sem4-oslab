#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 4 // Number of resources
#define N 5 // Number of threads
#define min(a,b) (a<b)?(a):(b)
#define print2d(x) \
    puts(#x": "); \
    for (int i = 0; i < N || (puts("")-1); i++) \
        for (int j = 0; j < M || (puts("")-1); j++) \
            printf("%d ", x[i][j]); \
    printf("\n");
#define print1d(x,y) \
    puts(#x": "); \
    for (int i = 0; i < y || (puts("\n")-2); i++) \
        printf("%d ", x[i]);

int available[M];
int maximum[N][M];
int allocation[N][M] = {{0}};
int need[N][M];
int finished[N] = {0};

int check_finished(int customer) { // O(M)
    if (!finished[customer]) {
        int flag=1;
        for (int i = 0; i < M; i++)
            flag *= (need[customer][i] == 0);
        return flag;
    } else
        return 1;
}

int check_safety(int request[]) { // O(M)
    // Prevent requesting more resources than available
    int flag=1;
    for (int i = 0; i < M; i++)
        flag *= (request[i] <= available[i]);
    return flag;
}

int check_need(int customer, int request[]) { // O(M)
    // Prevent requesting more resources than needed
    int flag=1;
    for (int i = 0; i < M; i++)
        flag *= (request[i] <= need[customer][i]);
    return flag;
}

int check_alloc(int customer, int release[]) { // O(M)
    // Check if release request is less than actual allocation
    int flag=1;
    for (int i = 0; i < M; i++)
        flag *= (release[i] <= allocation[customer][i]);
    return flag;
}

int request_resources(int customer, int request[]) {
    int cond1 = check_safety(request);
    int cond2 = check_need(customer, request);
    int cond3 = !finished[customer];

    if (cond1 && cond2 && cond3) {
        puts("Request has been accepted, resources allocated!");
        for (int i = 0; i < M; i++) {
            need[customer][i] -= request[i];
            allocation[customer][i] += request[i];
            available[i] -= request[i];
        }
        if (check_finished(customer)) {
            finished[customer] = 1;
            printf("Thread %d has finished execution!\n", customer);
        }
        return 0;
    } else {
        puts("Request has been denied!");
        if (!cond1) puts("Safe state is threatened!");
        if (!cond2) puts("Requesting more than needed!");
        if (!cond3) puts("Thread already finished!");
        return -1;
    }
}

void release_resources(int customer, int release[]) {
    if (check_alloc(customer, release)) {
        for (int i = 0; i < M; i++) {
            allocation[customer][i] -= release[i];
            available[i] += release[i];
        }
        printf("Resources released successfully!\n");
    } else {
        // Too many resources to release
        for (int i = 0; i < M; i++) {
            release[i] = min(release[i], allocation[customer][i]);
            allocation[customer][i] -= release[i];
            available[i] += release[i];
        }
        printf("Invalid request! Released resources are: ");
        for (int i = 0; i < M; i++)    {
            printf("%d ", release[i]);
        }
        puts("");
    }
}

void REPL() {
    while(1) {
        char cmd[5]; int cust_id; int args[M];
        printf("banker> ");
        scanf("%s", cmd);
        if (strcmp(cmd, "RQ") == 0) {
            scanf("%d", &cust_id);
            for (int i = 0; i < M; i++)
                scanf("%d", args+i);
            request_resources(cust_id, args);
        } else if (strcmp(cmd, "RL") == 0) {
            scanf("%d", &cust_id);
            for (int i = 0; i < M; i++)
                scanf("%d", args+i);
            release_resources(cust_id, args);
        } else if (strcmp(cmd, "*") == 0) {
            print2d(allocation); print2d(maximum); print2d(need);
            print1d(available, M);  print1d(finished, N);
        } else if (strcmp(cmd, "X") == 0) {
            exit(0);
        } else
            puts("Invalid Prompt!");
    }
}

int main(int argc, char const *argv[])    {
    FILE* file = fopen("9_maxres.txt", "r");
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)    {
            fscanf(file, "%d", &maximum[i][j]);
            need[i][j] = maximum[i][j];
        }
    fclose(file);

    if (argc == M+1) {
        for (int i = 0; i < M; i++)
            available[i] = atoi(argv[i+1]);
        REPL();
    } else
        printf("Invalid arguments!\n");
    return 0;
}

/*
a 10 5 7 8
*
RQ 0 2 2 2 2
*
RQ 0 5 5 5 5
RQ 0 4 2 5 1
*
RL 0 1 1 1 1
RL 0 9 9 9 9
*
RQ 0 5 5 5 55
X
*/
