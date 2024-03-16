#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N_THREADS 10

pthread_t tids[N_THREADS];

void* thread_func(void* arg) {
    int thread_num = *((int*)arg);
    free(arg);
    int s = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (s != 0) {
        perror("pthread_setcanceltype");
        exit(EXIT_FAILURE);
    }

    // Mientras el hilo esté vivo
    while (1) {
        printf("Hilo %d: Sigo vivo.\n", thread_num);
        sleep(1);
        int random = rand() % N_THREADS + 1;
        if (thread_num != random) {
            pthread_cancel(tids[random - 1]);
            printf("Hilo %d: He cancelado el hilo %d.\n", thread_num, random);
        }
    }

    return NULL;
}

int main() {
    int s;
    // Ciclo de creación de hilos
    for (int i = 0; i < N_THREADS; i++) {
        int* arg = malloc(sizeof(int));
        if (arg == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        *arg = i + 1;
        s = pthread_create(&tids[i], NULL, thread_func, arg);
        if (s != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Ciclo de espera
    for (int i = 0; i < N_THREADS; i++) {
        void* res;
        s = pthread_join(tids[i], &res);
        if (s != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        if (res == PTHREAD_CANCELED) {
            printf("Reporte: El hilo %d fue cancelado (%lu)\n", i + 1, tids[i]);
        }
    }

    return 0;
}
