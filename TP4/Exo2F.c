#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
    }

    char *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    printf("%s",addr);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
    }

    write(STDOUT_FILENO, addr, sb.st_size);

    if (munmap(addr, sb.st_size) == -1)
    {
        perror("munmap");
    }

    if (close(fd) == -1)
    {
        perror("close");
    }

    return EXIT_SUCCESS;
}
