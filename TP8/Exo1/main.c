#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>


typedef struct{
    pthread_mutex_t mutex;
    int count;
    int n;
    int open;
}barrier_t;

barrier_t* create_barrier(int N){
    int fd = shm_open("/mybarrier", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd, sizeof(barrier_t)) == -1) {
        perror("ftrncate");
        exit(EXIT_FAILURE);
    }

    barrier_t* barrier = mmap(NULL, sizeof(barrier_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (barrier == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    close(fd);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&barrier->mutex,&attr);
    barrier->count = 0;
    barrier->n = N;
    barrier->open = 0;
    return barrier;
}

void wait_barrier(barrier_t* barrier){
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if(barrier->count == barrier->n){
        barrier->open = 1;
    }
    pthread_mutex_unlock(&barrier->mutex);
    while(!barrier->open){
        usleep(100);
    }
    pthread_mutex_lock(&barrier->mutex);
    if(barrier->count != 0){
        barrier->count--;
    }
    pthread_mutex_unlock(&barrier->mutex);
}

void destroy_barrier(barrier_t* barrier) {
    pthread_mutex_destroy(&barrier->mutex);
    munmap(barrier, sizeof(barrier_t));
    shm_unlink("/mybarrier");
}

void *thread_function(void* arg){
    barrier_t* barrier =(barrier_t*)arg;
    int pid = getpid();
    int delay = rand() % 5;
    printf("Thread %d: waiting %d seconds\n", pid, delay);
    sleep(delay);
    printf("Thread %d: arrived at barrier\n", pid);
    wait_barrier(barrier);
    printf("Thread %d: passed barrier\n", pid);
    return NULL;}

int main(){
    pid_t pid;
    int N = 4;
    barrier_t* barrier = create_barrier(N);
    
    for(int i=0 ;i<N; i++){
        pid = fork();
        if(pid==-1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            thread_function(barrier);
                        exit(EXIT_SUCCESS);

        }
    }

    for(int i =0;i<N;i++){
        wait(NULL);
    }
    destroy_barrier(barrier);
    return 0;
}