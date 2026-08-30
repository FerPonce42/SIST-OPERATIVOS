#include <stdio.h>
//nuevas biblitoecas:
#include <sys/ipc.h> // para crear la COLA de mensajes.
#include <sys/msg.h> // funciones para enviar recibir mensaje.
#include <string.h> // textazo p 

//Definir la KEY:
#define KEY_COLA 123456

// estructura del mensaje que recibiremos.

struct MiMensaje{

    long mtype;
    char texto[100];
};




int main(){


    // para ubicar la cola del kernel
    int msqid;
    msqid = msgget(KEY_COLA, 0666);


    //obejeto del mensaje
    struct MiMensaje mensaje_recepcion;


    while(1){
        //el PRIMER CERO,  corresponde al TIPO del mensaje. El del final ya corresponde a la funcion.
        msgrcv(msqid, &mensaje_recepcion, sizeof(mensaje_recepcion.texto), 0, 0);

        printf("Mensaje recibido [Tipo %ld]: %s\n", mensaje_recepcion.mtype, mensaje_recepcion.texto);


    }


}