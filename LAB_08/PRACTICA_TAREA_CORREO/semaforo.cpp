// Programa: crear u obtener el semáforo.
// Uso:  ./semaforo c   -> crea el semáforo y lo inicia
//       ./semaforo g   -> solo obtiene el id de uno ya creado
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define CLAVE_SEM 0x1111   // identifica al semáforo; todos los programas usan la misma

// OBLIGATORIA: semctl espera esta unión exactamente así. No modificar.
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
union semun u;

// {nº de semáforo en el conjunto, operación, flags}
struct sembuf op_P = {0, -1, 0};   // P: esperar (resta 1, bloquea si es 0)
struct sembuf op_V = {0, +1, 0};   // V: liberar (suma 1)

void semP(int id) {
    if (semop(id, &op_P, 1) < 0) { perror("semop P"); exit(13); }
}

void semV(int id) {
    if (semop(id, &op_V, 1) < 0) { perror("semop V"); exit(14); }
}

// Crea el semáforo (si no existe) y devuelve su id
int crearSem() {
    int i = semget(CLAVE_SEM, 2, 0666 | IPC_CREAT);   // NUEVO (CV): 2 semáforos (antes 1)
    if (i < 0) { perror("semget"); exit(11); }
    return i;
}

// Obtiene el id de un semáforo que ya existe
int obtenerSem() {
    int i = semget(CLAVE_SEM, 2, 0);                  // NUEVO (CV): 2 semáforos (antes 1)
    if (i < 0) { perror("semget"); exit(11); }
    return i;
}

// Fija el valor inicial del semáforo 0 (1 = un solo proceso a la vez)
void iniciarSem(int id, int numRecursos) {
    u.val = numRecursos;
    if (semctl(id, 0, SETVAL, u) < 0) { perror("semctl"); exit(12); }
}

// NUEVO (CV): inicia el semáforo 1 (cola de espera de la CV) en 0
void iniciarCola(int id) {
    u.val = 0;
    if (semctl(id, 1, SETVAL, u) < 0) { perror("semctl"); exit(12); }
}

int main(int argc, char *argv[]) {
    int id = -1;

    if (argc < 2) {
        printf("Uso: %s c|g\n", argv[0]);
        return 1;
    }

    if (argv[1][0] == 'c') {
        id = crearSem();
        iniciarSem(id, 1);
        iniciarCola(id);                              // NUEVO (CV)
    }
    if (argv[1][0] == 'g') {
        id = obtenerSem();
    }

    printf("sem:%d\n", id);
    return 0;
}
