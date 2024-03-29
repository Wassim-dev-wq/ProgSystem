#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int initialiser_mutex(pthread_mutex_t *pmutex) {
  pthread_mutexattr_t mutexattr;

  if (pthread_mutexattr_init(&mutexattr) != 0) {
    perror("mutexattr init");
    return 1;
  }

  if (pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED) != 0) {
    perror("mutexattr setpshared");
    return 1;
  }

  if (pthread_mutex_init(pmutex, &mutexattr) != 0) {
    perror("mutex init");
    return 1;
  }
  return 0;
}


int main() {
  /* attention : cet exercice est là pour vous convaincre que dans le cas de plusieurs
     processus il faut mettre le mutex dans la mémoire partagée, pas dans la pile des
     processus, ce n'est donc pas un exemple à suivre */
  pthread_mutex_t mutex;

  if (initialiser_mutex(&mutex)!=0) {
    perror("initialiser");
    exit(1);
  }

  if (fork() !=0 ) { // père
    pthread_mutex_lock(&mutex);
    pause();
  } else { // fils
    sleep(1);
    pthread_mutex_lock(&mutex);
    printf("je suis passé :)\n");
  }
  return 0;
}
/* Since the mutex is allocated on the stack, both the parent and child processes have their own separate copies of the mutex. 
When the child process attempts to lock the mutex, it is locking its own private copy, not the one locked by the parent process. 
As a result, the child process can lock the mutex and print the "je suis passé :)" message even though the parent process has locked its own copy of the mutex. */