#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()    {
    long int status = syscall(546);
    printf("System call sys_hello returned %ld\n", status);
    return 0;
}
