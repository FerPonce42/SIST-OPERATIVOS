#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#define CLAVE 0x1111

/* Estructura que permite configurar los valores del semáforo */
union semun
{
    int valor;              /* Valor que se asignará al semáforo */
    struct semid_ds *buf;   /* Información y configuración del semáforo */
    unsigned short *array;  /* Arreglo para configurar varios semáforos */
};

/* Variable global utilizada para configurar el semáforo */
union semun arg;

/* Operación P: resta 1 al semáforo y espera si no hay recursos disponibles */
struct sembuf P = {0, -1, 0};

/* Operación V: suma 1 al semáforo para liberar un recurso */
struct sembuf V = {0, +1, 0};

/* Solicita acceso al recurso controlado por el semáforo */
void semP(int sem_id)
{
    if (semop(sem_id, &P, 1) < 0)
    {
        perror("semop p");
        exit(13);
    }
}

/* Libera el recurso controlado por el semáforo */
void semV(int sem_id)
{
    if (semop(sem_id, &V, 1) < 0){
        perror("semop p");
        exit(14);
    }
}

/* Busca el semáforo y lo crea si todavía no existe */
int getSemCreate()
{
    int sem_id = semget(CLAVE, 1, 0666 | IPC_CREAT);

    if (sem_id < 0)
    {
        perror("semget");
        exit(11);
    }

    return sem_id;
}

/* Busca un semáforo que ya debe existir, sin crearlo */
int getSem()
{
    int sem_id = semget(CLAVE, 1, 0);

    if (sem_id < 0)
    {
        perror("semget");
        exit(11);
    }

    return sem_id;
}

/* Establece el valor del semáforo según la cantidad de recursos */
void controlSem(int sem_id, int cant_recursos)
{
    arg.valor = cant_recursos;

    if (semctl(sem_id, 0, SETVAL, arg) < 0)
    {
        perror("semctl");
        exit(12);
    }
}

int main(int argc, char *argv[])
{
    char tecla;
    int sem_id;

    /* Si el primer argumento comienza con 'c',
       buscamos el semáforo o lo creamos si no existe.
       Después, lo inicializamos con valor 1.
    */
    if (argv[1][0] == 'c')
    {
        sem_id = getSemCreate();
        controlSem(sem_id, 1);
    }

    /* Si el primer argumento comienza con 'g',
       buscamos un semáforo que ya exista.
    */
    if (argv[1][0] == 'g')
    {
        sem_id = getSem();
    }

    printf("sem:%d\n", sem_id);

    /* Obtenemos la cadena de texto del segundo argumento */
    char *texto = argv[2];
    int largo = strlen(texto);

    for (int i = 0; i < largo; ++i)
    {
        semP(sem_id);              /* entramos a la zona crítica */

        putchar(texto[i]);
        fflush(stdout);

        sleep(rand() % 2);

        putchar(texto[i]);
        fflush(stdout);

        /* Si la cadena comienza con 'a', esperamos
           a que el usuario introduzca un carácter.
        */
        if (argv[2][0] == 'a')
            scanf(" %c", &tecla);

        semV(sem_id);              /* salimos de la zona crítica */

        sleep(rand() % 2);
    }
}