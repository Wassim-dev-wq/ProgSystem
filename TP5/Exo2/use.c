#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
    int fd = open("libop.so", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    int (*f)(int, int) = mmap(NULL, 1024, PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);
    if (f == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // Execute loadadd
    system("./loadadd");
    printf("Result after loading add: %d\n", f(32, 10));

    // Execute loadmult
    system("./loadmult");
    printf("Result after loading mult: %d\n", f(32, 10));

    munmap((void *)f, 1024);
    close(fd);

    return EXIT_SUCCESS;
}
 