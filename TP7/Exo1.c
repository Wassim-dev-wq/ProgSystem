#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define N 10

int main()
{
    // Ouvre le sémaphore "libres" avec une valeur initiale de N
    sem_t *libres = sem_open("/libres", O_CREAT, 0644, N);
    if (libres == SEM_FAILED) {
        perror("Erreur lors de l'ouverture du sémaphore 'libres'");
        exit(EXIT_FAILURE);
    }

    // Ouvre le sémaphore "occupes" avec une valeur initiale de 0
    sem_t *occupes = sem_open("/occupes", O_CREAT, 0644, 0);
    if (occupes == SEM_FAILED) {
        perror("Erreur lors de l'ouverture du sémaphore 'occupes'");
        exit(EXIT_FAILURE);
    }

    // Crée la zone de mémoire partagée pour le tampon circulaire
    int fd = shm_open("/tampon", O_CREAT | O_RDWR, 0644);
    if (fd == -1) {
        perror("Erreur lors de la création de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    // Définit la taille de la mémoire partagée
    if (ftruncate(fd, N * sizeof(int)) == -1) {
        perror("Erreur lors du redimensionnement de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    // Mappe la mémoire partagée dans l'espace d'adressage du processus
    int *tampon = mmap(NULL, N * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (tampon == MAP_FAILED) {
        perror("Erreur lors de l'attachement de la mémoire partagée");
        exit(EXIT_FAILURE);
    }

    // Initialise les curseurs
    int curseur_lecture = 0;
    int curseur_ecriture = 0;

    // Écrit des entiers aléatoires dans le tampon circulaire
    for (int i = 0; i < 2 * N; i++) {
        // Attends qu'il y ait une place libre dans le tampon
        sem_wait(libres);

        // Écrit un entier aléatoire dans le tampon
        tampon[curseur_ecriture] = random();

        // Incrémente le curseur d'écriture
        curseur_ecriture = (curseur_ecriture + 1) % N;

        // Libère une place occupée dans le tampon
        sem_post(occupes);
    }

    // Ferme le sémaphore "libres"
    if (sem_close(libres) == -1) {
        perror("Erreur lors de la fermeture du sémaphore 'libres'");
        exit(EXIT_FAILURE);
    }

    // Ferme le sémaphore "occupes"
    if (sem_close(occupes) == -1) {
        perror("Erreur lors de la fermeture du sémaphore 'occupes'");
        exit(EXIT_FAILURE);
    }

    // Supprime le sémaphore "libres"
    if (sem_unlink("/libres") == -1) {
        perror("Erreur lors de la suppression du sémaphore
