#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>   // agregado: exit

#define TAM_BLOQUE 27

int main()            // agregado: int
{
    int shm_id;
    key_t clave;
    char *shm, *cursor;

    /* Necesitamos el bloque "5678", creado por el server */
    clave = 5678;

    /* Buscar el bloque (sin IPC_CREAT) */
    if ((shm_id = shmget(clave, TAM_BLOQUE, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* Conectar el bloque a nuestro espacio */
    if ((shm = shmat(shm_id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /* Leer lo que puso el server */
    for (cursor = shm; *cursor != '\0'; cursor++)   // cambiado: era NULL
        putchar(*cursor);
    putchar('\n');

    /* Avisar con '*' que ya leímos */
    *shm = '*';

    exit(0);
}