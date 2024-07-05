#include <stdio.h>
#include <stdlib.h>
#define PQ0 {.front=0, .back=0}

// CUSTOM DATA TYPES
typedef struct {
    char pname[10];
    int burst_ms, arrival, priority;
} Process;

typedef struct {
    Process Parray[1024];
    int front;
    int back;
} Pqueue;

// GLOBAL DATA
Process
    p1 = {"P1", 20, 0, 1},
    p2 = {"P2", 10, 3, 2},
    p3 = {"P3", 40, 4, 5},
    p4 = {"P4", 20, 50, 3},
    p5 = {"P5", 30, 60, 4},
    p6 = {"P6", 20, 70, 3};

Pqueue Q_ = {.front=0,.back=0};


void PUSH(Pqueue* Q, Process* P) {
    Q->Parray[Q->back] = *P;
    Q->back++;
}

Process* POP(Pqueue* Q) {
    Process* P = Q->Parray+Q->front;
    Q->front++;
    return P;
}

void FILL(Pqueue* Q) {
    PUSH(Q, &p1); PUSH(Q, &p2); PUSH(Q, &p3);
    PUSH(Q, &p4); PUSH(Q, &p5); PUSH(Q, &p6);
}

int compare(Process A, Process B, int sort_key) {
    switch(sort_key) {
        case 1:
        return A.arrival < B.arrival;
        case 2:
        return A.burst_ms < B.burst_ms;
        case 3:
        return A.priority > B.priority;
    }
    return -1;
}

void sort(Pqueue* Q, int sort_key) {
    int len = Q->back - Q->front;
    for (int i = 0; i < len; i++)    {
        for (int j = 0; j < len; j++)    {
            Process A = Q->Parray[Q->front + i];
            Process B = Q->Parray[Q->front + j];
            if (compare(A, B, sort_key)) {
                Process P = Q->Parray[Q->front + j];
                Q->Parray[Q->front + j] = Q->Parray[Q->front + i];
                Q->Parray[Q->front + i] = P;
            }
        }
    }
}

void FCFS(Pqueue* Q) {
    double TAT=0, BT=0;
    sort(Q, 1);
    for (int curr_time = 0; Q->front != Q->back; curr_time++)    {
        Process P = *POP(Q); BT += P.burst_ms;
        if (P.arrival <= curr_time) {
            printf("Current Time: %d ms\n", curr_time);
            printf("Process %s arrived at %d\n", P.pname, P.arrival);
            printf("Burst: %d\n", P.burst_ms);
            printf("Completed at: %d\n\n", P.burst_ms + --curr_time);
            curr_time += P.burst_ms;
            TAT += curr_time - P.arrival;
        }
    }
    TAT /= 6; BT /= 6;
    printf("Average Turnaround time: %lf\n", TAT);
    printf("Average Waiting time: %lf\n\n", TAT-BT);
}

void SJF(Pqueue* Q) {
    double TAT=0, BT=0;
    sort(Q, 2);
    for (int curr_time = 0; Q->front != Q->back; curr_time++)    {
        Process P = *POP(Q); BT += P.burst_ms;
        if (curr_time < P.arrival)
            curr_time = P.arrival;

        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst: %d\n", P.burst_ms);
        printf("Completed at: %d\n\n", P.burst_ms + --curr_time);
        curr_time += P.burst_ms;
        TAT += curr_time - P.arrival;
    }
    TAT /= 6; BT /= 6;
    printf("Average Turnaround time: %lf\n", TAT);
    printf("Average Waiting time: %lf\n\n", TAT-BT);
}

void PRIORITY(Pqueue* Q) { // NON-PRE-EMPTIVE
    double TAT=0, BT=0;
    sort(Q, 1); // Sort by arrival
    Pqueue waiting = PQ0;
    int prev_start =0;
    int curr_time = 0;

    while(Q->front != Q->back) {
        if (waiting.front == waiting.back) {
            // NO waiting processes
            Process P = *POP(Q); BT += P.burst_ms;
            prev_start = curr_time;
            if (curr_time < P.arrival)
            curr_time = P.arrival;
            
            printf("Current Time: %d ms\n", curr_time);
            printf("Process %s arrived at %d\n", P.pname, P.arrival);
            printf("Burst: %d\nPriority: %d\n", P.burst_ms, P.priority);

            while (curr_time < prev_start + P.burst_ms) {
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
                curr_time++;
            }
            printf("Completed at: %d\n\n", --curr_time);
            TAT += curr_time - P.arrival;
            
        } else {
            // Waiting queue is not empty
            // Sort waiting queue by priority
            sort(&waiting, 3);

            Process P = *POP(&waiting);
            prev_start = curr_time;
            if (curr_time < P.arrival)
            curr_time = P.arrival;
            
            printf("Current Time: %d ms\n", curr_time);
            printf("Process %s arrived at %d\n", P.pname, P.arrival);
            printf("Burst: %d\nPriority: %d\n", P.burst_ms, P.priority);

            while (curr_time < prev_start + P.burst_ms && Q->front != Q->back) {
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
                curr_time++;
            }
            curr_time = prev_start + P.burst_ms;
            printf("Completed at: %d\n\n", --curr_time);
            TAT += curr_time - P.arrival;

        }
    curr_time++;  
    }

    while(waiting.front != waiting.back) {
        sort(&waiting, 3);

        Process P = *POP(&waiting);
        prev_start = curr_time;
        
        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst: %d\nPriority: %d\n", P.burst_ms, P.priority);

        curr_time += P.burst_ms;
        printf("Completed at: %d\n\n", --curr_time);
        TAT += curr_time - P.arrival;
        curr_time++;
    }
    TAT /= 6; BT /= 6;
    printf("Average Turnaround time: %lf\n", TAT);
    printf("Average Waiting time: %lf\n\n", TAT-BT);
}

void ROUNDROBIN(Pqueue* Q) {
    double TAT=0, BT=0;
    sort(Q, 1); // Sort by arrival
    int curr_time=0;
    int quantum = 10;
    Pqueue waiting = PQ0;

    while (Q->front != Q->back) {
        if (waiting.front == waiting.back) {
            // Waiting Queue is empty
            Process P = *POP(Q); BT += P.burst_ms;
            printf("Current Time: %d ms\n", curr_time);
            printf("Process %s arrived at %d\n", P.pname, P.arrival);
            printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);

            if (P.burst_ms <= quantum) {
                curr_time += P.burst_ms;
                printf("Process %s Completed at: %d\n\n",P.pname, curr_time);
                TAT += curr_time - P.arrival;
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
            } else {
                P.burst_ms -= quantum;
                curr_time += quantum;
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
                PUSH(&waiting, &P);
            }
        } else {
            Process P = *POP(&waiting);
            printf("Current Time: %d ms\n", curr_time);
            printf("Process %s arrived at %d\n", P.pname, P.arrival);
            printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);

            if (P.burst_ms <= quantum) {
                curr_time += P.burst_ms;
                printf("Process %s Completed at: %d\n\n",P.pname, curr_time);
                TAT += curr_time - P.arrival;
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
            } else {
                P.burst_ms -= quantum;
                curr_time += quantum;
                if (Q->Parray[Q->front].arrival <= curr_time) {
                    PUSH(&waiting, POP(Q));
                }
                PUSH(&waiting, &P);
            }
        }
    }

    while (waiting.front != waiting.back) {
        Process P = *POP(&waiting);
        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);

        if (P.burst_ms <= quantum) {
            curr_time += P.burst_ms;
            printf("Process %s Completed at: %d\n\n", P.pname, curr_time);
            TAT += curr_time - P.arrival;
        } else {
            P.burst_ms -= quantum;
            curr_time += quantum;
            PUSH(&waiting, &P);
        }    
    }
    TAT /= 6; BT /= 6;
    printf("Average Turnaround time: %lf\n", TAT);
    printf("Average Waiting time: %lf\n\n", TAT-BT);
}

void MULTILEVELQ(Pqueue* Q) {
    // We ignore default priority. 5 most, 1 least
    // Q => RR10, then RR20, then FCFS
    double TAT=0, BT=0; 
    sort(Q, 1); Process P; int curr_time=0;
    Pqueue _RR20=PQ0, _FCFS=PQ0;

    while(Q->front != Q->back) {
        P = *POP(Q); BT += P.burst_ms;
        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);

        if (P.burst_ms <= 10) {
            curr_time += P.burst_ms;
            printf("Process %s Completed at: %d\n\n",P.pname, curr_time);
            TAT += curr_time - P.arrival;
            P.priority=5;
            printf("Priority of Process %s set to 5\n", P.pname);
        } else {
            P.burst_ms -= 10;
            curr_time += 10;
            PUSH(&_RR20, &P);
            P.priority=3;
            printf("Priority of Process %s set to 3\n", P.pname);
        }
    }

    while(_RR20.front != _RR20.back) {
        P = *POP(&_RR20);
        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);

        if (P.burst_ms <= 20) {
            curr_time += P.burst_ms;
            printf("Process %s Completed at: %d\n\n",P.pname, curr_time);
            TAT += curr_time - P.arrival;
            P.priority=3;
            printf("Priority of Process %s set to 3\n", P.pname);
        } else {
            P.burst_ms -= 20;
            curr_time += 20;
            P.priority=1;
            printf("Priority of Process %s set to 1\n", P.pname);
            PUSH(&_FCFS, &P);
        }
    }

    while(_FCFS.front != _FCFS.back) {
        P = *POP(&_FCFS);
        P = *POP(&_RR20);
        printf("Current Time: %d ms\n", curr_time);
        printf("Process %s arrived at %d\n", P.pname, P.arrival);
        printf("Burst Remaining at start of %d ms: %d ms\n\n", curr_time, P.burst_ms);
        curr_time += P.burst_ms;
        printf("Process %s Completed at: %d\n\n",P.pname, curr_time);
        TAT += curr_time - P.arrival;
    }

    TAT /= 6; BT /= 6;
    printf("Average Turnaround time: %lf\n", TAT);
    printf("Average Waiting time: %lf\n\n", TAT-BT);
}

int main(int argc, char const *argv[])    {
    
    puts("FIRST COME FIRST SERVE\n");
    FILL(&Q_); FCFS(&Q_);
    puts("_________________________________________");
    getchar();

    puts("SHORTEST JOB FIRST\n");
    FILL(&Q_); SJF(&Q_);
    puts("_________________________________________");
    getchar();

    puts("PRIORITY SCHEDULING\n");
    FILL(&Q_); PRIORITY(&Q_);
    puts("_________________________________________");
    getchar();

    puts("ROUND ROBIN SCHEDULING\n");
    FILL(&Q_); ROUNDROBIN(&Q_);
    puts("_________________________________________");
    getchar();

    puts("MULTILEVEL QUEUE SCHEDULING\n");
    FILL(&Q_); MULTILEVELQ(&Q_);
    puts("_________________________________________");
    getchar();

    return 0;
}
