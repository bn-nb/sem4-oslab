#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#define MIN_PID 300
#define MAX_PID 5000

int* bitmap;

int allocate_map(void) {
    bitmap = (int*)malloc(sizeof(int)*(MAX_PID - MIN_PID + 1));
    if (bitmap == NULL) {
        return -1;
    }

    for (int pid = MIN_PID; pid <= MAX_PID; pid++) {
        bitmap[pid - MIN_PID] = 0;
    }
    return 1;
}

int allocate_pid(void) {
    for (int pid = MIN_PID; pid <= MAX_PID; pid++) {
        if (!bitmap[pid - MIN_PID]) {
            bitmap[pid - MIN_PID] = 1;
            return pid;
        }
    }
    return -1;
}

void release_pid(int pid) {
    printf("Process ID %d released!\n", pid);
    bitmap[pid - MIN_PID] = 0;
}

int main(int argc, char **argv) {
    int status = allocate_map();
    if (status > 0) {
        pid_t process1 = allocate_pid();
        pid_t process2 = allocate_pid();
        printf("Process ID of Process 1: %d\n", process1);
        printf("Process ID of Process 2: %d\n", process2);

        release_pid(process1);
        release_pid(process2);
    }
    return 0;
}
