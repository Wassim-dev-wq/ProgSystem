#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#define ITERATIONS 100000

int *var;
pthread_mutex_t mutex;

void *increment(void *arg) {
    int i;
    for (i = 0; i < ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        (*var)++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    var = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (var == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    *var = 0;
    pthread_mutex_init(&mutex, NULL);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Processus fils
        pthread_t tid1, tid2;
        pthread_create(&tid1, NULL, increment, NULL);
        pthread_create(&tid2, NULL, increment, NULL);
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        exit(0);
    } else {
        // Processus parent
        pthread_t tid1, tid2;
        pthread_create(&tid1, NULL, increment, NULL);
        pthread_create(&tid2, NULL, increment, NULL);
        pthread_join(tid1, NULL);
        pthread_join(tid2, NULL);
        wait(NULL);
        printf("Valeur finale : %d\n", *var);
        pthread_mutex_destroy(&mutex);
        munmap(var, sizeof(int));
        exit(0);
    }
}
