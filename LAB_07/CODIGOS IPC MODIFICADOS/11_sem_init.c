#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

/* Bibliotecas necesarias para trabajar con semáforos del sistema */
#include <sys/ipc.h>
#include <sys/sem.h>

/* Unión utilizada para enviar valores y datos al semáforo */
union semun {
    int valor;             /* Valor inicial del semáforo */
    struct semid_ds *buf;  /* Información y configuración del semáforo */
    ushort *array;         /* Arreglo de valores de varios semáforos */
};

int main(void)
{
    key_t clave;      /* Clave para identificar el conjunto de semáforos */
    int sem_id;       /* Identificador del conjunto de semáforos */
    union semun arg;  /* Variable para configurar el semáforo */

    /* Asignamos una clave fija para identificar el semáforo */
    clave = 0x4444;

    /*
    Alternativa para generar la clave a partir de un archivo.
    Actualmente está desactivada porque se encuentra entre comentarios.

    if ((clave = ftok("seminit.c", 'J')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    */

    /* Creamos un conjunto que contiene un solo semáforo.
       Si ya existe, utilizamos el conjunto existente.
       0666 establece los permisos de acceso.
    */
    if ((sem_id = semget(clave, 1, 0666 | IPC_CREAT)) == -1)
    {
        perror("semget");
        exit(1);
    }

    /* Establecemos el valor inicial del semáforo en 1 */
    arg.valor = 1;

    /* Asignamos ese valor al primer semáforo del conjunto */
    if (semctl(sem_id, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        exit(1);
    }

    return 0;
}