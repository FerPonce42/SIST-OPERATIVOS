#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CLAVE 12345
#define TAM_BLOQUE 100

int main(){

    int shm_id = shmget(CLAVE, TAM_BLOQUE, 0666); // aqui ya no es necesario CREAR solo revisar si existe  = 0666

    if(shm_id < 0){
        perror("Error con shmget");
        exit(1);
    }

    char* shm = shmat(shm_id, NULL, 0);

    if(shm == (char*)-1){
        perror("Error con shmat");
        exit(1);      
    }


    // para leer

    //forma 1: 
/*
    for(int i = 0 ; shm[i] != '\0' ; i++){
        putchar(shm[i]);
    }

*/


    //forma 2:

    printf("%s\n",shm);

// avisarle al productor que ya leyo
    shm[0] = '*';


    //desconectar del bloque

    shmdt(shm);

}