/*
Vamos con Message Passing (Message Queue).

Vas a escribir un programa que:

Define un struct para el mensaje (con mtype + contenido).
Crea/obtiene una cola con msgget.
Manda un mensaje con msgsnd.
Lo recibe con msgrcv (en el mismo programa, para simplificar — más adelante podrías separarlo en dos).

Paso 1: Ya viste esto en el código que me compartiste antes (el de tu Lab 02). Intenta escribir SOLO el struct del mensaje, sin nada más:

c
struct MiMensaje {
    // ¿qué campos necesita?
};

Recuerda: necesita un campo obligatorio para el tipo (que se llama específicamente mtype, ese nombre es fijo, no puedes cambiarlo porque 
msgsnd/msgrcv lo buscan por ese nombre exacto), y un campo para el contenido real del mensaje. ¿Qué tipo le pondrías a cada uno?

*/

#include <stdio.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

#define KEY 123


struct Mensaje{

    long mtype;
    char contenido[100];

};


int main(){


    int id_cola = msgget(KEY, IPC_CREAT | 0666);


    struct Mensaje m;

    m.mtype = 1;

    strcpy(m.contenido, "Quiero dormir");
   

    msgsnd(id_cola, &m, sizeof(m.contenido), 0);


    struct Mensaje r;

    msgrcv(id_cola, &r, sizeof(r.contenido), 1, 0);

    printf("%s\n",r.contenido);


}