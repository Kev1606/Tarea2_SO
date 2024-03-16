#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __unix__
#include <unistd.h>
#elif defined _WIN32
#include "windows.h"
#define sleep(x) Sleep(1000 * x)
#endif

typedef struct alarm_tag {
    int seconds;
    char message[64];
} alarm_t;

void *thread_A(void *arg) {
    sleep(10);
    printf("Hilo A: He terminado de dormir por 10 segundos.\n");

    pthread_t *thread_B = (pthread_t *)arg;
    printf("Hilo A: Esperando a que termine el Hilo B...\n");
    pthread_join(*thread_B, NULL);
    printf("Hilo A: El Hilo B ha terminado.\n");
    return NULL;
}

void *thread_B(void *arg) {
    sleep(20);
    printf("Hilo B: He terminado de dormir por 20 segundos.\n");

    pthread_t *thread_C = (pthread_t *)arg;
    printf("Hilo B: Esperando a que termine el Hilo C...\n");
    pthread_join(*thread_C, NULL);
    printf("Hilo B: El Hilo C ha terminado.\n");
    return NULL;
}

void *thread_C(void *arg) {
    sleep(30);
    printf("Hilo C: He terminado de dormir por 30 segundos.\n");
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threadA, threadB, threadC;
    int status;

    status = pthread_create(&threadC, NULL, thread_C, NULL);
    status = pthread_create(&threadB, NULL, thread_B, &threadC);
    status = pthread_create(&threadA, NULL, thread_A, &threadB);

    pthread_join(threadA, NULL);

    return 0;
}
