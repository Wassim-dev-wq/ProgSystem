#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main()
{
    int i;
    srandom(time(NULL));
    for (i = 0; i < 10; i++)
    {
        if (fork() == 0)
        {
            int sleepTime = random() % 10 + 1;
            printf("P%d pid : %d\n", i + 1, getpid());
            sleep(sleepTime);
            printf("P%d Terminé aprés %d secondes\n", i + 1,sleepTime);

            return 0;
        }
    }
    return 0;
}
