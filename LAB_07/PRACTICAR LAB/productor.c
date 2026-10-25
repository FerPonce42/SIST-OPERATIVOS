#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CLAVE 12345
#define TAM_BLOQUE 100

int main(){

    int shm_id = shmget(CLAVE, TAM_BLOQUE, IPC_CREAT | 0666);

    if(shm_id < 0){
        perror("Error con shmget");
        exit(1);
    }


    char* shm = shmat(shm_id, NULL, 0);

    if(shm == (char*)-1){
        perror("Error con shmat");
        exit(1);
    }


    int i = 0;
    for(char c = 'a'; c <= 'z' ; c++, i++){
        shm[i] = c;
    }
    
    shm[i] = '\0'; // esto me sirve para marcar un fin


    // esperar que el consumidor termine de leer
    while(1){

        if(shm[0] == '*'){
            break;
        }

        sleep(2);
    }

    //desconectar del bloque

    shmdt(shm);

    // liberar bloque
    shmctl(shm_id, IPC_RMID, NULL);

}

/*
Etapa	  | Función	              | Quién
---------------------------------------------
Crear	    shmget con IPC_CREAT	productor
Buscar	    shmget sin IPC_CREAT	consumidor
Conectar	shmat	los dos
Usar	    shm[i]	los dos
Desconectar	shmdt	los dos
Borrar	    shmctl con IPC_RMID	    productor

*/