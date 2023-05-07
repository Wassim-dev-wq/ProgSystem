#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define N 10

int main(){

    sem_t *libres = sem_open("/libres",O_CREAT,0644, N);
    sem_t *occupes = sem_open("/occupes",O_CREAT,0644,0);
    int fd = shm_open("/tampon",O_CREAT | O_RDWR, 0644);
    ftruncate(fd, N * sizeof(int);
    
}