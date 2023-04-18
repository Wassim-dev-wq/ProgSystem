#include <unistd.h>
#include <stdio.h>

int main()
{
    int profondeur = 0;
    pid_t p1, p2, p3, p4, p5, p6, p7;
    p1 = getpid();
    printf("P1 : pid = %d , Pere pid = %d\n", p1, getppid());
    p2 = fork();
    if (p2 == 0)
    {
        printf("P2 : pid = %d , Pere pid = %d\n", getpid(), getppid());
        pause();
    }
    p3 = fork();
    if (p3 == 0)
    {
        printf("P3 : pid = %d , Pere pid = %d\n", getpid(), getppid());
        pause();
        p4 = fork();
        if (p4 == 0)
        {
            printf("P4 : pid = %d , Pere pid = %d\n", getpid(), getppid());
            pause();
            p5 = fork();
            if (p5 == 0)
            {
                printf("P5 : pid = %d , Pere pid = %d\n", getpid(), getppid());
                pause();
            }
        }
        p6 = fork();
        if (p6 == 0)
        {
            printf("P6 : pid = %d , Pere pid = %d\n", getpid(), getppid());
            pause();
        }
    }
    p7 = fork();
    if (p7 == 0)
    {
        printf("P7 : pid = %d , Pere pid = %d\n", getpid(), getppid());
        pause();
    }
}