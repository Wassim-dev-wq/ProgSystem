#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    int fd = open("libop.so", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    int (*f)(int, int) = mmap(NULL, 1024, PROT_READ | PROT_EXEC, MAP_SHARED, fd, 0);
    if ((void *)f == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    printf("f(32, 10) = %d\n", f(32, 10));

    munmap(f, 1024);
    close(fd);

    return 0;
}
