#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x1111

/* Estructura que permite configurar los valores del semáforo */
union semun
{
    int val;                /* Valor que se asignará al semáforo */
    struct semid_ds *buf;   /* Información y configuración del semáforo */
    unsigned short *array;  /* Arreglo para configurar varios semáforos */
};

/* Variable global utilizada para configurar el semáforo */
union semun u;

/* Operación P: resta 1 al semáforo y espera si no hay recursos disponibles */
struct sembuf p = {0, -1, 0};

/* Operación V: suma 1 al semáforo para liberar un recurso */
struct sembuf v = {0, +1, 0};

/* Solicita acceso al recurso controlado por el semáforo */
void semP(int id)
{
    /* Ejecutamos la operación P sobre el semáforo */
    if (semop(id, &p, 1) < 0)
    {
        perror("semop p");
        exit(13);
    }
}

/* Libera el recurso controlado por el semáforo */
void semV(int id)
{
    /* Ejecutamos la operación V sobre el semáforo */
    if (semop(id, &v, 1) < 0){
        perror("semop p");
        exit(14);
    }
}

/* Busca el semáforo y lo crea si todavía no existe */
int getSemCreate()
{
    int i = semget(KEY, 1, 0666 | IPC_CREAT);

    if (i < 0)
    {
        perror("semget");
        exit(11);
    }

    /* Devolvemos el identificador del semáforo */
    return i;
}

/* Busca un semáforo que ya debe existir, sin crearlo */
int getSem()
{
    int i = semget(KEY, 1, 0);

    if (i < 0)
    {
        perror("semget");
        exit(11);
    }

    /* Devolvemos el identificador del semáforo encontrado */
    return i;
}

/* Establece el valor del semáforo según la cantidad de recursos */
void controlSem(int id, int numRecursos)
{
    /* Indicamos el valor que tendrá el semáforo */
    u.val = numRecursos;

    /* Aplicamos el nuevo valor al semáforo */
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }
}

int main(int argc, char *argv[])
{
    char c;
    int id;

    /* Si el primer argumento comienza con 'c',
       buscamos el semáforo o lo creamos si no existe.
       Después, lo inicializamos con valor 1.
    */
    if (argv[1][0] == 'c')
    {
        id = getSemCreate();
        controlSem(id, 1);
    }

    /* Si el primer argumento comienza con 'g',
       buscamos un semáforo que ya exista.
    */
    if (argv[1][0] == 'g')
    {
        id = getSem();
    }

    /* Mostramos el identificador del semáforo utilizado */
    printf("sem:%d\n", id);

    /* Obtenemos la cadena de texto del segundo argumento */
    char *s = argv[2];
    int l = strlen(s);

    /* Recorremos cada carácter de la cadena */
    for (int i = 0; i < l; ++i)
    {
        /* Solicitamos acceso antes de utilizar el recurso */
        semP(id);

        /* Imprimimos el carácter actual */
        putchar(s[i]);
        fflush(stdout);

        /* Esperamos entre 0 y 1 segundo */
        sleep(rand() % 2);

        /* Volvemos a imprimir el mismo carácter */
        putchar(s[i]);
        fflush(stdout);

        /* Si la cadena comienza con 'a', esperamos
           a que el usuario introduzca un carácter.
        */
        if (argv[2][0] == 'a')
            scanf(" %c", &c);

        /* Liberamos el recurso para que otro proceso pueda utilizarlo */
        semV(id);

        /* Esperamos entre 0 y 1 segundo antes de continuar */
        sleep(rand() % 2);
    }
}