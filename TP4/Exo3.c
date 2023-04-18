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

char cesar(char c, int offset)
{
    if (c >= 'a' && c <= 'z')
        return (char)('a' + ((c - 'a' + offset) % 26));
    if (c >= 'A' && c <= 'Z')
        return (char)('A' + ((c - 'A' + offset) % 26));
    return c;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1)
        handle_error("open");

    struct stat sb;
    if (fstat(fd, &sb) == -1)
        handle_error("fstat");
    char *addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handle_error("mmap");

    for (size_t i = 0; i < sb.st_size; i++)
    {
        addr[i] = cesar(addr[i], 3); // décalage de 3 caractères
    }

    if (munmap(addr, sb.st_size) == -1)
        handle_error("munmap");

    if (close(fd) == -1)
        handle_error("close");
    return EXIT_SUCCESS;
}
