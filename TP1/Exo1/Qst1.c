#include <stdio.h>
#include <unistd.h>

int main() {
    int i;
    pid_t pid = fork();
    printf("pid = %d\n",pid);
    if (pid == 0) {
        for (i = 0; i < 1000 ; i++) {
            printf("miaou!\n");
        }
    } else {
        for (i = 0; i < 1000; i++) {
            printf("ouaf!\n");
        }
    }

    return 0;
}
