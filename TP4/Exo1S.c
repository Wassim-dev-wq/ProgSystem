#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdatomic.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char **argv)
{
    // TODO
    int var = 0;
    pid_t pid;
    //int *pvar = 0;
    atomic_int *pvar = 0;
    
    // shared memory using mmap
    pvar = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    if (pvar == MAP_FAILED)
    {
        perror("mmap failed");
    }
    if ((pid = fork()) == 0)
    { // fork succed
        for (int i = 0; i < 100000; i++)
        {
            (*pvar)++;
        }
        exit(0);
    }
    else if (pid < 0)
    {
        perror("Fork failed"); // failed to fork
    }
    else
    {
        int status;
        for (int i = 0; i < 100000; i++)
        {
            (*pvar)++;
        }
        waitpid(pid, &status, 0);
        if (status == 0)
        {
            printf("var : %d\n", var);
            printf("pvar : %d\n", *pvar);
        }
        else if (status == 1)
        {
            perror("Child process");
        }
    }
    if (munmap(pvar, sizeof(int)) == -1)
    {
        perror("munmap failed");
    }
}