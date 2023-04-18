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
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s src dest\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd_src = open(argv[1], O_RDONLY);
    if (fd_src == -1)
        handle_error("open src file");

    int fd_dest = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1)
        handle_error("open dest file");

    struct stat sb;
    if (fstat(fd_src, &sb) == -1)
        handle_error("fstat");

    if (ftruncate(fd_dest, sb.st_size) == -1)
        handle_error("ftruncate");

    char *addr_src = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd_src, 0);
    if (addr_src == MAP_FAILED)
        handle_error("mmap");

    char *addr_dest = mmap(NULL, sb.st_size, PROT_WRITE, MAP_SHARED, fd_dest, 0);
    if (addr_dest == MAP_FAILED)
        handle_error("mmap");

    if (close(fd_src) == -1)
        handle_error("close src file");
    if (close(fd_dest) == -1)
        handle_error("close dest file");

    return EXIT_SUCCESS;
}
