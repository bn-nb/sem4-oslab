#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int strToInt(const char* string) {
    int i=0, j=0, arg=0;
    while(string[i]) {
        j = string[i];
        if (j > 47 && j < 58) {
            arg = 10 * arg + (j-48);
        } else {
            printf("Positive Integers Only !\n");
            exit(0);
        }
        i++;
    }
    return arg;
}

int main(int argc, char const *argv[])    {
    if (argc == 2) {
        int   arg = strToInt(argv[1]);
        pid_t p11 = fork();

        if (p11 > 0)
            wait(NULL);
        else {
            while (arg > 1) {
                printf("%d ", arg);
                if (arg%2)  arg = 3*arg+1;
                else        arg = arg/2;
            }
            printf("1\n");
            exit(0);
        }

    } else {
        printf("Exactly one argument needed !\n");
    }
    return 0;
}
