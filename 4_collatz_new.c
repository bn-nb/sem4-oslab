#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

double microsecs() {
    struct timeval now; gettimeofday(&now, NULL);
    return now.tv_sec + now.tv_usec / 1e6;
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

void collatz(int arg) {
    printf("| %4d: %4d ", getpid(), arg);
    // Flush buffer after each output
    // To copy only empty buffer to child
    // Prevents Redundant outputs
    // Copy-On-Write Memory
    fflush(stdout);
    if (arg != 1) {
        pid_t p11 = fork();
        if (p11 > 0)
            wait(NULL);
        else
            collatz( (arg % 2) ? (3*arg + 1) : (arg/2) );
    }
}

int main(int argc, char const *argv[])    {
    double time = microsecs();
    if (argc == 2) {
        int p12 = fork();
        if (p12 > 0) {
            wait(NULL); time = microsecs() - time;
            printf("|\n%lf secs taken\n____\n", time);
        } else {
            // Call function from child of main to prevent the children 
            // created inside collatz() from returning to main
            collatz( strToInt(argv[1]) );
        }
    } else
        printf("Exactly one argument needed !\n");
    return 0;
}
