// Programa: escribe los números IMPARES (1,3,5...99) en la memoria compartida.
// Ping-pong con el otro escritor: semáforo (mutex) + variable condicional (turno).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "cv.h"                // NUEVO (CV)

const int TAM_SHM = TAM_SHM_TOTAL;   // NUEVO (CV): texto + estado de la CV (antes 2000)
const key_t CLAVE_SHM = 5678;
#define CLAVE_SEM 0x1111

// OBLIGATORIA: semctl espera esta unión exactamente así. No modificar.
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
union semun u;

struct sembuf op_P = {0, -1, 0};   // P: esperar
struct sembuf op_V = {0, +1, 0};   // V: liberar

void semP(int id) {
    if (semop(id, &op_P, 1) < 0) { perror("semop P"); exit(13); }
}

void semV(int id) {
    if (semop(id, &op_V, 1) < 0) { perror("semop V"); exit(14); }
}

// Obtiene el id del semáforo ya creado (con ./semaforo c)
int obtenerSem() {
    int i = semget(CLAVE_SEM, 2, 0);   // NUEVO (CV): 2 semáforos (antes 1)
    if (i < 0) { perror("semget"); exit(11); }
    return i;
}

int main() {
    char copia[2001];   // copia local del contenido de la memoria
    char numero[10];    // aquí se arma el texto ".N"
    int shmid;
    char *shm;

    // Crear (o abrir si ya existe) la memoria compartida
    if ((shmid = shmget(CLAVE_SHM, TAM_SHM, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    // Conectarla (cast obligatorio en C++)
    if ((shm = (char *) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    EstadoCV *est = estadoCV(shm);   // NUEVO (CV)

    int id = obtenerSem();
    printf("sem:%d\n", id);

    for (int i = 1; i < 100; i = i + 2) {
        semP(id);                                  // entrar a la sección crítica
            while (est->turno != 1)               // NUEVO (CV): ¿es mi turno?
                cvEsperar(id, est);                // NUEVO (CV): si no, me duermo

            strcpy(copia, shm);                    // leer lo que hay
            printf("LOOP=%d  %s\n", i, copia);
            snprintf(numero, sizeof(numero), ".%d", i);   // i=33 -> ".33"
            strcat(shm, numero);                   // concatenar al final

            est->turno = 0;                       // NUEVO (CV): ahora le toca al otro
            cvSenalar(id, est);                    // NUEVO (CV): lo despierto
        semV(id);                                  // salir de la sección crítica
    }
    return 0;
}
