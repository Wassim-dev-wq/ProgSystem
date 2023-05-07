#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigalrm_handler(int signum) {
    printf("Timeout reached, terminating process...\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: timeout <command>\n");
        exit(EXIT_FAILURE);
    }

    // register the SIGALRM signal handler
    struct sigaction sa_sigalrm;
    sa_sigalrm.sa_handler = sigalrm_handler;
    sigemptyset(&sa_sigalrm.sa_mask);
    sa_sigalrm.sa_flags = 0;
    if (sigaction(SIGALRM, &sa_sigalrm, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // set the alarm for 5 seconds
    alarm(5);

    // execute the command
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // child process
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // parent process
        // wait for the child process to exit
        int status;
        waitpid(pid, &status, 0);

        // cancel the alarm
        alarm(0);

        if (WIFEXITED(status)) {
            printf("Command exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Command terminated by signal %d\n", WTERMSIG(status));
        }
    }

    return 0;
}
