// Programa: deja la memoria compartida vacía.
// La memoria ya debe existir (la crean los escritores).
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "cv.h"                // NUEVO (CV)

const int TAM_SHM = 27;        // tamaño que pedimos (debe ser <= al del segmento creado)
const key_t CLAVE_SHM = 5678;  // identifica la memoria compartida

int main() {
    int shmid;
    char *shm, *s;

    // Buscar el segmento existente
    if ((shmid = shmget(CLAVE_SHM, TAM_SHM, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Conectar el segmento a nuestro espacio de memoria
    // (C++ exige el cast de void* a char*)
    if ((shm = (char *) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Recorrer el texto actual y borrarlo con espacios
    for (s = shm; *s != '\0'; s++)
        *s = ' ';

    // Cadena vacía: el primer carácter es el fin de texto
    *shm = '\0';

    // NUEVO (CV): reiniciar el estado de la CV
    EstadoCV *est = estadoCV(shm);
    est->turno = 0;
    est->esperando = 0;

    return 0;
}
