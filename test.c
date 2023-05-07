#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd = open("a", O_RDWR | O_CREAT | O_TRUNC, 0666);
    pid_t pid = fork();

    if (pid > 0) {
        // Parent process        
        write(fd, "toto", 4);

    } else {
        // Child process
        write(fd, "titi", 4);
        close(fd);
    }
    return 0;
}