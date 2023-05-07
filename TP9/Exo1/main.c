#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigusr1_count = 0;

void sigusr1_handler(int signum) {
    if (sigusr1_count == 0) {
        printf("Humm...\n");
    }
    else {
        printf("Merci mon prince!\n");
        exit(EXIT_SUCCESS);
    }
    sigusr1_count++;
}

int main() {
    // register the signal handler for SIGUSR1
    struct sigaction sa_sigusr1;
    sa_sigusr1.sa_handler = sigusr1_handler;
    sigemptyset(&sa_sigusr1.sa_mask);
    sa_sigusr1.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa_sigusr1, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // wait for two SIGUSR1 signals
    printf("Waiting for SIGUSR1 signal...\n");
    while (sigusr1_count < 2) {
        sleep(1);
    }

    return 0;
}
