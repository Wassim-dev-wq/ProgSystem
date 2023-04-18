#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    char bruit[8];
    int i;
    pid_t pid = fork();
    printf("pid = %d\n",pid);
    if (pid == 0) {
        strcpy(bruit,"miaou!");
        for (i = 0; i < 1000 ; i++) {
            printf("%s, add = %p\n",bruit,(void *)&bruit);
        }
    } else {
        strcpy(bruit,"ouaf!");
        for (i = 0; i < 1000; i++) {
            printf("%s, add = %p\n",bruit,(void *)&bruit);
        }
    }

    return 0;
}
