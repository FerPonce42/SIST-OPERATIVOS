#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>


#include <sys/sem.h>

#define CLAVE 6789

// parametro: que semaforo , que hacer (sumar o restar), extra
struct sembuf P = {0, -1 , 0};

struct sembuf V = {0, +1 , 0};


int main(){

    int cant_sem = 1;

    int sem_id = semget(CLAVE, cant_sem, 0666);

    if(sem_id < 0){
        perror("Error con semget");
        exit(1);
    }


    int ope_P = semop(sem_id, &P, 1);

    if(ope_P < 0){
        perror("Error con semop -> P()");
        exit(1);   
    }
//-------------------------------------------
//              ZONA CRITICA

    printf("Estoy en ZONA CRITICA\n");
    sleep(5);

//-------------------------------------------
    int ope_V = semop(sem_id, &V, 1);

    if(ope_V < 0){
        perror("Error con semop -> V()");
        exit(1);   
    }

}