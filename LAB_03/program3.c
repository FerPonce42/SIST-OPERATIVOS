#include <stdio.h>
#include <unistd.h> // Necesario para sleep(5)
//nuevas biblitoecas:
#include <sys/ipc.h> // para crear la COLA de mensajes.
#include <sys/msg.h> // funciones para enviar recibir mensaje.
#include <string.h> // textazo p 

//Definir la KEY:
#define KEY_COLA2 654321

// estructura del mensaje que recibiremos.

struct MiMensaje{

    long mtype;
    char texto[100];
};

int main(){

    int msqid = msgget(KEY_COLA2, IPC_CREAT | 0666);

    struct MiMensaje mensaje_recepcion;

    while(1){

        msgrcv(msqid, &mensaje_recepcion, sizeof(mensaje_recepcion.texto), 4, 0);

        printf("Mensaje recibido [Tipo %ld]: %s\n", mensaje_recepcion.mtype, mensaje_recepcion.texto);

        sleep(5);
    }

    return 0;
}