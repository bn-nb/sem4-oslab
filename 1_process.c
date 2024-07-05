#include <stdio.h>
#include <stdlib.h>
#include <signal.h>    // kill
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // wait
#include <unistd.h>    // fork, sleep

int main(int argc, char const *argv[])    {

    pid_t p11 = fork();

    if (p11 == 0) {

        // Child Process
        printf("\nInside 1st Child of Main");
        printf(" with PID: %d", getpid());
        printf("\nParent's PID: %d", getppid());

        printf("\nExiting child process");
        printf(" with ID: %d", getpid());
        printf("\nThis is a Zombie as parent is sleeping");
        printf("\n\n");

        sleep(0.5); // to avoid collisions
        kill(getpid(),5);
        
    } else if (p11 > 0) {

        // Main Process
        sleep(5); 
        printf("\n\nInside Main Process");
        printf(" with PID: %d\n", getpid());
        printf("Parent's (of Main) PID: %d\n", getppid());

        pid_t p12 = fork();
        
        if (p12 > 0) {

            // Main Process
            wait(NULL);
            printf("Main Process %d is exiting...", getpid());
            printf("\n\n");
            exit(0);

        } else if (p12 == 0) {

            // Child Process
            sleep(2); // to avoid collisions

            // Orphan may be adopted by grandparents
            // to keep root free

            printf("\nInside 2nd Child of "); 
            printf("Main with PID: %d", getpid());

            printf("\nParent's PID: %d\n", getppid());
            printf("This is an Orphan Process\n\n");

            pid_t p13 = fork();

            if (p13 > 0) {
                kill(getpid(), 1);exit(0);
            } else if (p13 == 0) {
                
                sleep(2); // to avoid collisions

                printf("\n\nInside Child of 2nd Child ");
                printf("of Main with PID: %d\n", getpid());
                
                printf("Parent's PID: %d\n", getppid());
                printf("This is an Orphan Process\n\n");

            } else { printf("Error creating processes"); }
        } else { printf("Error creating processes"); }
    } else { printf("Error creating processes"); }
    return 0;
}

// https://www.programiz.com/c-programming/online-compiler/
