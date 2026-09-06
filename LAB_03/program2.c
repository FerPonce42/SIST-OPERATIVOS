#include <stdio.h>
#include <unistd.h>
//nuevas biblitoecas:
#include <sys/ipc.h> // para crear la COLA de mensajes.
#include <sys/msg.h> // funciones para enviar recibir mensaje.
#include <string.h> // textazo p 

//Definir la KEY:
#define KEY_COLA1 123456
#define KEY_COLA2 654321

// estructura del mensaje que recibiremos.

struct MiMensaje{

    long mtype;
    char texto[100];
};

int main(){

    int msqid1 = msgget(KEY_COLA1, IPC_CREAT | 0666);
    int msqid2 = msgget(KEY_COLA2, IPC_CREAT | 0666);

    struct MiMensaje mensaje_recepcion;

    while(1){

        msgrcv(msqid1, &mensaje_recepcion, sizeof(mensaje_recepcion.texto), 3, 0);

        mensaje_recepcion.mtype = 4;
        msgsnd(msqid2, &mensaje_recepcion, sizeof(mensaje_recepcion.texto), 0);

        sleep(1);
    }

    return 0;
}