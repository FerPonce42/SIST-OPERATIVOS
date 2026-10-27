#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>   // agregado: exit
#include <unistd.h>   // agregado: sleep

#define TAM_BLOQUE 27

int main()            // agregado: int
{
    char c;
    int shm_id;
    key_t clave;
    char *shm, *cursor;

    /* Le ponemos clave "5678" a nuestro bloque */
    clave = 5678;

    /* Crear el bloque */
    if ((shm_id = shmget(clave, TAM_BLOQUE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* Conectar el bloque a nuestro espacio */
    if ((shm = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /* Poner datos para que el otro proceso los lea */
    cursor = shm;

    for (c = 'a'; c <= 'z'; c++)
        *cursor++ = c;
    *cursor = '\0';   // cambiado: era NULL

    /* Esperar a que el otro proceso cambie el primer carácter a '*' */
    while (*shm != '*')
        sleep(1);

    exit(0);
}