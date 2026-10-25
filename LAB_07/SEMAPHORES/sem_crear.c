#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>

#include <sys/sem.h>

#define CLAVE 6789


// esta estructura, el SO la quiere asi, y ni modo p.

union semun{

    int valor;
    struct semid_ds *buf;
    unsigned short *array;

};

int main(){

    int cant_sem = 1;

    int sem_id = semget(CLAVE, cant_sem, IPC_CREAT | 0666);

    if(sem_id < 0){
        perror("Error con semget");
        exit(1);
    }

    // aca inicializo ese union (que parece struct pero, no es... bueno si es pero no)
    union semun arg;
    arg.valor = 1;

    int sem_ctl = semctl(sem_id, 0, SETVAL, arg);


    if(sem_ctl < 0){
        perror("Error con semctl");
        exit(1);
    }
}