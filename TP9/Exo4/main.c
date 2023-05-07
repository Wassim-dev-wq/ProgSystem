#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>

#define NUM_CHILDREN 9

int counters[NUM_CHILDREN + 1] = {0};

void sigint_handler(int signum) {
    pid_t pid = getpid();
    for (int i = 0; i < NUM_CHILDREN + 1; i++) {
        if (getpid() == pid) {
            printf("Process %d: counter value = %d\n", i, counters[i]);
        }
    }
}

void sigusr2_handler(int signum) {
    pid_t pid = getpid();
    printf("Process %d: Gnark, j'ai gagné !\n", pid);
    for (int i = 1; i <= NUM_CHILDREN; i++) {
        if (getpid() != pid) {
            printf("Process %d: Ah, j'ai perdu !\n", i);
        }
    }
    exit(EXIT_SUCCESS);
}

int main() {
    srand(time(NULL));

    // create the child processes
    pid_t pids[NUM_CHILDREN];
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            // child process
            // set up SIGINT handler
            struct sigaction sa_sigint;
            sa_sigint.sa_handler = sigint_handler;
            sigemptyset(&sa_sigint.sa_mask);
            sa_sigint.sa_flags = 0;
            if (sigaction(SIGINT, &sa_sigint, NULL) == -1) {
                perror("sigaction");
                exit(EXIT_FAILURE);
            }

            // set up SIGUSR2 handler
            struct sigaction sa_sigusr2;
            sa_sigusr2.sa_handler = sigusr2_handler;
            sigemptyset(&sa_sigusr2.sa_mask);
            sa_sigusr2.sa_flags = 0;
            if (sigaction(SIGUSR2, &sa_sigusr2, NULL) == -1) {
                perror("sigaction");
                exit(EXIT_FAILURE);
            }

            // initialize counter
            counters[i + 1] = 0;

            // wait for SIGUSR1 signal from parent
            pause();

            // increment counter and sleep for a random amount of time
            while (counters[i + 1] < 20) {
                counters[i + 1]++;
                sleep(rand() % 3 + 1);
            }

            // send SIGUSR2 signal to other processes
            for (int j = 0; j < NUM_CHILDREN; j++) {
                if (pids[j] != getpid()) {
                    kill(pids[j], SIGUSR2);
                }
            }

            // exit child process
            exit(EXIT_SUCCESS);
        } else {
            // parent process
            pids[i] = pid;
        }
    }

    // wait for all child processes to be created
    sleep(1);

    // send SIGUSR1 signal to all child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        kill(pids[i], SIGUSR1);
    }

    // wait for all child processes to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // exit parent process
    exit(EXIT_SUCCESS);
}
