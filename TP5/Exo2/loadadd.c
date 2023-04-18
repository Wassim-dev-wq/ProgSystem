#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    int fd = open("libop.so", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ftruncate(fd, 1024);

    void *map = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    memcpy(map, add, 1024);

    munmap(map, 1024);
    close(fd);

    printf("Function add() copied to libop.so\n");

    return 0;
}
