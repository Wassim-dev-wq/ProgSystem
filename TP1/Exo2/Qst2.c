#include <unistd.h>
#include <stdio.h>

int main()
{
    int profondeur = 0;
    pid_t p1, p2, p3, p4, p5, p6, p7;

    p1 = getpid();
    printf("P1 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", p1, getppid(), profondeur, profondeur);
    profondeur++;

    p2 = fork();
    if (p2 == 0)
    {
        printf("P2 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
        pause();
        return 0;
    }

    p3 = fork();
    if (p3 == 0)
    {
        printf("P3 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
        profondeur++;

        p4 = fork();
        if (p4 == 0)
        {
            printf("P4 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
            profondeur++;
            p5 = fork();
            if (p5 == 0)
            {
                printf("P5 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
                pause();
                return 0;
            }
            pause();
            return 0;
        }
        p6 = fork();
        if (p6 == 0)
        {
            printf("P6 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
            pause();
            return 0;
        }
        pause();
        return 0;
    }

    p7 = fork();
    if (p7 == 0)
    {
        printf("P7 : pid = %d, parent pid = %d, profondeur = %d, parent profondeur = %d\n", getpid(), getppid(), profondeur, profondeur - 1);
        pause();
        return 0;
    }
    return 0;
}
