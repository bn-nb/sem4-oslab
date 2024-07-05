#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int  memSize;
    int* start;
    int  pid;
} allocMap;

allocMap processes[1024];
int procSize;

int *memory;

typedef int(MemFit)(int,int,int);
MemFit firstFit, bestFit, worstFit;

void memFill(allocMap allocation) {
    for(int i=0; i < allocation.memSize; i++) {
        allocation.start[i] = allocation.pid;
    }
}

void memAlloc(int pid, int size, int N, char fit) {
    int flag=-1;
    switch(fit) {
        case 'F': flag=firstFit(pid, size, N); break;
        case 'B': flag=bestFit(pid, size, N); break;
        case 'W': flag=worstFit(pid, size, N); break;
        default: puts("Invalid Fit");
    }
    if (flag) puts("Memory allocation unsuccessful!");
    else puts("Memory allocation successful!");
}

void memFree(int pid) {
    int flag=0;
    for (int i = 0; i < procSize; i++)    {
        if (processes[i].pid == pid) {
            processes[i].pid = -1;
            memFill(processes[i]);
            flag=1;
        }
    }
    if (flag) puts("Memory released!");
    else puts("Could not release memory");
}

int firstFit(int pid, int size, int N) {
    // First suitable block of memory
    for (int i = 0; i < N; i++) 
        if (memory[i] == -1) {
            int j=i;
            while (++j < N && memory[j] == -1) {
                if (j-i == size-1) {
                    allocMap temp = {.memSize=size, .start=memory+i, .pid=pid};
                    memFill(temp); processes[procSize++] = temp;
                    return 0;
                }
            }
        }
    return -1;
}

int bestFit(int pid, int size, int N) {
    // Smallest suitable block of memory
    int blockFound = 0, minSize, blockIndex;

    for (int i = 0; i < procSize; i++)    {
        if (processes[i].pid != -1) continue;
        if (processes[i].memSize < size) continue;
        if ((blockFound) && (processes[i].memSize > minSize)) continue;
        blockFound = 1;
        blockIndex = i;
        minSize = processes[i].memSize;
    }

    if (blockFound) {
        processes[blockIndex].pid = pid;
        memFill(processes[blockIndex]);
        return 0;
    } else {
        return firstFit(pid, size, N);
    }
}

int worstFit(int pid, int size, int N) {
    // Largest block of memory
    int* start = processes[procSize-1].start + processes[procSize-1].memSize;
    allocMap temp = {.start = start, .memSize=size, .pid=pid};
    memFill(temp);
    processes[procSize++] = temp;
    return 0;
}

void status(int N) {
    for (int i=0; i < N; ) {
        int value = memory[i];
        printf("Address: [%d, ", i);
        while((++i < N) && (memory[i] == value));
        if (value==-1) printf("%d] : Free\n", i-1);
        else printf("%d] : PID : %d\n", i-1, value);
    }
}

void compact(int N) {
    for (int i=0; i < procSize-1; i++) {
        if (processes[i].pid == -1) {
            int j=i;
            while (processes[++j].pid == -1)
                // All Succeeding blocks are free
                if (j == procSize-1) return;

            // j is first occupied space 
            // after bunch of free space
            int k=i, l=j;
            for (; l < procSize; k++, l++)    {
                processes[k] = processes[l];
                processes[l].pid = -1;
                if (k==0)
                    processes[k].start = memory;
                else
                    processes[k].start 
                        = processes[k-1].start
                        + processes[k-1].memSize;
                memFill(processes[k]);
                memFill(processes[l]);
            }
        }
    }
    puts("Memory Compacted! Status:");
    status(N);
}

void REPL(int N) {
    while(1) {
        char cmd[5], proc[5], fit;
        int mem;
        printf("alloc> ");
        scanf("%s", cmd);
        if (strcmp(cmd, "RQ") == 0) {
            scanf("%s %d %c", proc, &mem, &fit);
            memAlloc(atoi(&proc[1]), mem, N, fit);
        } else if (strcmp(cmd, "RL") == 0) {
            scanf("%s", proc);
            memFree(atoi(&proc[1]));
        } else if (strcmp(cmd, "C") == 0) {
            compact(N);
        } else if (strcmp(cmd, "STAT") == 0) {
            status(N);
        } else if (strcmp(cmd, "X") == 0) {
            exit(0);
        } else 
            printf("Invalid Prompt!\n");
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 2) {
        procSize = 0;
        memset(processes, 0, 1024 * sizeof(allocMap));
        int N = atoi(argv[1]);
        memory = (int*) malloc(N * sizeof(int));
        if (memory) {
            memset(memory, -1, N * sizeof(int));
            REPL(N);
        } else puts("Memory Allocation failed");
    } else puts("Invalid Arguments!");
    return 0;
}

/*
a 50
RQ P0 5 F
RQ P1 4 F
RQ P2 3 F
RQ P3 4 F
STAT
RL P0
RL P2
STAT
RQ P4 3 B
RQ P5 5 W
STAT
C
X
*/
