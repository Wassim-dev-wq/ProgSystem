#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int mult(int a, int b) {
    return a * b;
}

int main() {
    int fd = open("libop.so", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    void *map = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    memcpy(map, mult, 1024);

    munmap(map, 1024);
    close(fd);

    printf("Function mult() copied to libop.so\n");

    return 0;
}
