#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>   // strtok, strcmp
#include <unistd.h>   // fork, execvp
#include <sys/wait.h> // wait
#define CHUNK_SIZE 64

char*** hist;

char** parseLine(void) {
    // Read input line and return array of tokens
    // We need this memory till end of program
    int index = 0, size = CHUNK_SIZE;

    char* buffer = malloc(sizeof(char) * CHUNK_SIZE);
    while (1) {
        char c = getchar();
        if (c == EOF || c == '\n') break;
        buffer[index++] = c;
        if (index+1 > size) {
            size += (sizeof(char) * CHUNK_SIZE);    
            buffer = realloc(buffer, size);
        }
    }
    buffer[index] = 0; // NULL termination   
    index = 0; size = CHUNK_SIZE;

    char** args = malloc(sizeof(char*) * CHUNK_SIZE);
    char* token = strtok(buffer, " \t");
    while (token) {
        args[index++] = token;
        token = strtok(NULL, " \t");
        if (index+1 > size) {
            size += (sizeof(char*) * CHUNK_SIZE);   
            args = realloc(args, size);
        }
    }
    args[index++] = NULL; // NULL termination
    return args;
}

void printHist() {
    int i=0;
    while(hist[i]) {
        int j=0;
        while (hist[i][j])
            printf("%s ", hist[i][j++]);
        puts(""); i++;
    }
}

void execute(char** args) {
    if (strcmp(args[0], "cd") == 0)
        chdir(args[1]);
    else if (strcmp(args[0], "exit") == 0)
        exit(0);
    else if (strcmp(args[0], "hist") == 0)
        printHist(hist);
    else {
        pid_t p11 = fork();
        if (p11 > 0) wait(NULL);
        else execvp(args[0], args);
    }
}

int main(int argc, char const *argv[])    {
    puts("Shell in Linux using C            !");
    puts("Built-in commands: cd, hist, exit !");   
    puts("All other linux commands work too !");

    hist = malloc(sizeof(char**) * CHUNK_SIZE);
    int index=0, size=CHUNK_SIZE;

    while (1) {
        printf("shell> ");
        char** args = parseLine();
        // Can not free (for history)
        hist[index++] = args;
        hist[index] = NULL;
        if (index+1 > size) {
            size += sizeof(char**) * CHUNK_SIZE;
            hist = realloc(hist, size);
        }
        execute(args);
    }
    return 0;
}
