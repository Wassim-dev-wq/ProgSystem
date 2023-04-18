#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int *initialisation(int N) {
    // Create a shared memory region using mmap
    int *map = mmap(NULL, (N+1) * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    // Check if mmap failed
    if (map == MAP_FAILED) {
        perror("mmap");
        return MAP_FAILED;
    }

    // Initialize the shared memory with correct values
    // 0 and 1 are not prime, so set their values to 0
    map[0] = 0;
    map[1] = 0;
    // For the remaining elements, set their values to 1
    for (int i = 2; i <= N; i++) {
        map[i] = 1;
    }

    return map;
}

void affiche(int *map, int N) {
    for (int i = 2; i <= N; i++) {
        if (map[i] == 1) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void raye_multiples(int *map, int N, int p) {
    for (int i = p * 2; i <= N; i += p) {
        map[i] = 0;
    }
}


int main() {
    int N = 100;
    int *map = initialisation(N);

    if (map == MAP_FAILED) {
        fprintf(stderr, "Shared memory initialization failed.\n");
        return 1;
    }

    for (int i = 2; i <= N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) { // Child process
            if (map[i] == 1) {
                raye_multiples(map, N, i);
            }
            exit(0);
        }
    }

    // Wait for all child processes to complete
    for (int i = 2; i <= N; i++) {
        wait(NULL);
    }

    // Display the list of prime numbers
    affiche(map, N);

    // Clean up the shared memory
    munmap(map, (N+1) * sizeof(int));

    return 0;
}