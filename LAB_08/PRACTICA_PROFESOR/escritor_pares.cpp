// Programa: escribe los números PARES (0,2,4...98) en la memoria compartida.
// Hace ping-pong con el otro escritor usando el semáforo (exclusión mutua).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

const int TAM_SHM = 2000;
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
    int i = semget(CLAVE_SEM, 1, 0);
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

    int id = obtenerSem();
    printf("sem:%d\n", id);

    for (int i = 0; i < 100; i = i + 2) {
        semP(id);                                  // entrar a la sección crítica
            strcpy(copia, shm);                    // leer lo que hay
            printf("loop=%d  %s\n", i, copia);
            snprintf(numero, sizeof(numero), ".%d", i);   // i=33 -> ".33"
            strcat(shm, numero);                   // concatenar al final
        semV(id);                                  // salir de la sección crítica
    }
    return 0;
}
