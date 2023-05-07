#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INT 100

int count = 0;
int sum = 0;
int min = MAX_INT;
int max = 0;

void sigint_handler(int signum) {
    printf("Statistics:\n");
    printf("Count: %d\n", count);
    printf("Sum: %d\n", sum);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
}

void sigterm_handler(int signum) {
    printf("Final statistics:\n");
    printf("Count: %d\n", count);
    printf("Sum: %d\n", sum);
    printf("Min: %d\n", min);
    printf("Max: %d\n", max);
    exit(EXIT_SUCCESS);
}

int main() {
    // register the signal handlers
    struct sigaction sa_sigint;
    sa_sigint.sa_handler = sigint_handler;
    sigemptyset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = 0;
    if (sigaction(SIGINT, &sa_sigint, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa_sigterm;
    sa_sigterm.sa_handler = sigterm_handler;
    sigemptyset(&sa_sigterm.sa_mask);
    sa_sigterm.sa_flags = 0;
    if (sigaction(SIGTERM, &sa_sigterm, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // create the pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // fork the child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        // close the read end of the pipe
        close(fd[0]);

        // write random integers to the pipe
        for (int i = 0; i < 10; i++) {
            int num = rand() % (MAX_INT + 1);
            write(fd[1], &num, sizeof(int));
        }

        // close the write end of the pipe
        close(fd[1]);
        // exit the child process
        sleep(5);

        exit(EXIT_SUCCESS);
    } else {
        // parent process
        // close the write end of the pipe
        close(fd[1]);

        // read integers from the pipe and update statistics
        int num;
        while (read(fd[0], &num, sizeof(int)) > 0) {
            count++;
            sum += num;
            if (num < min) {
                min = num;
            }
            if (num > max) {
                max = num;
            }
        }

        // close the read end of the pipe
        close(fd[0]);

        // wait for the child process to exit
        wait(NULL);
    }

    return 0;
}
