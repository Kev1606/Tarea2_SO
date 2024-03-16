#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_MENSAJE 100

struct Message {
    long type;
    char text[MAX_MENSAJE];
};

int main() {
    key_t clave = ftok(".", 'M');
    int cola_id = msgget(clave, IPC_CREAT | 0666);

    // Dos procesos hijos
    for (int i = 0; i < 2; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            struct Message message;
            message.type = i + 1; // Tipo de mensaje (1 o 2)
            snprintf(message.text, MAX_MENSAJE, "Hola, soy el proceso hijo #%d", i + 1);

            // Enviar el mensaje a la cola
            if (msgsnd(cola_id, &message, sizeof(struct Message), 0) == -1) {
                perror("Error al enviar el mensaje");
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < 2; ++i) {
        wait(NULL);
    }

    // Leer los mensajes de la cola
    struct Message received_message;
    for (int i = 0; i < 2; ++i) {
        msgrcv(cola_id, &received_message, sizeof(struct Message), i + 1, 0);
        printf("Proceso %d: %s\n", i + 1, received_message.text);
    }

    // Eliminar la cola de mensajes
    msgctl(cola_id, IPC_RMID, NULL);

    printf("Proceso padre ha terminado.\n");
    return 0;
}
