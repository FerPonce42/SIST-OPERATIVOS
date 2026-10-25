#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

/* Bibliotecas necesarias para trabajar con semáforos del sistema */
#include <sys/ipc.h>
#include <sys/sem.h>

/* Unión utilizada para enviar valores y datos al semáforo */
union semun {
    int val;               /* Valor inicial del semáforo */
    struct semid_ds *buf;  /* Información y configuración del semáforo */
    ushort *array;         /* Arreglo de valores de varios semáforos */
};

int main(void)
{
    key_t key;       /* Clave para identificar el conjunto de semáforos */
    int semid;       /* Identificador del conjunto de semáforos */
    union semun arg; /* Variable para configurar el semáforo */

    /* Asignamos una clave fija para identificar el semáforo */
    key = 0x4444;

    /*
    Alternativa para generar la clave a partir de un archivo.
    Actualmente está desactivada porque se encuentra entre comentarios.

    if ((key = ftok("seminit.c", 'J')) == -1)
    {
        perror("ftok");
        exit(1);
    }
    */

    /* Creamos un conjunto que contiene un solo semáforo.
       Si ya existe, utilizamos el conjunto existente.
       0666 establece los permisos de acceso.
    */
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1)
    {
        perror("semget"); /* Mostramos el error si falla la creación */
        exit(1);          /* Terminamos el programa */
    }

    /* Establecemos el valor inicial del semáforo en 1 */
    arg.val = 1;

    /* Asignamos ese valor al primer semáforo del conjunto */
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl"); /* Mostramos el error si falla la configuración */
        exit(1);          /* Terminamos el programa */
    }

    return 0; /* Indicamos que el programa terminó correctamente */
}