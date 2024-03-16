#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NUM_HIJOS 10

struct sharedMemory {
    int num1;
    int num2;
};

int main() {
    key_t clave = ftok(".", 'M');
    int shmid = shmget(clave, sizeof(struct sharedMemory), IPC_CREAT | 0666);
    struct sharedMemory *memoria = (struct sharedMemory *)shmat(shmid, NULL, 0);

    // 10 procesos hijos
    for (int i = 0; i < NUM_HIJOS; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // Código del proceso hijo
            srand(getpid());
            // Números aleatorios entre 0 y 99
            memoria->num1 = rand() % 100;
            memoria->num2 = rand() % 100;
            printf("Hijo %d: %d + %d = %d\n", i + 1, memoria->num1, memoria->num2, memoria->num1 + memoria->num2);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < NUM_HIJOS; ++i) {
        wait(NULL);
    }

    // Desvincular y eliminar la memoria compartida
    shmdt(memoria);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Proceso padre ha terminado.\n");
    return 0;
}
