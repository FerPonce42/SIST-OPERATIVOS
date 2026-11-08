// Programa: imprime lo que hay en la memoria compartida.
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

const int TAM_SHM = 27;
const key_t CLAVE_SHM = 5678;

int main() {
    int shmid;
    char *shm, *s;

    // Buscar el segmento existente
    if ((shmid = shmget(CLAVE_SHM, TAM_SHM, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Conectar el segmento (cast obligatorio en C++)
    if ((shm = (char *) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // Imprimir carácter por carácter hasta el fin de texto
    for (s = shm; *s != '\0'; s++)
        putchar(*s);
    putchar('\n');

    return 0;
}
