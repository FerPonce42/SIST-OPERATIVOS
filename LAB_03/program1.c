/*
  EMISOR
Fernando Ponce

*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <fcntl.h>
//nuevas biblitoecas:
#include <sys/ipc.h> // para crear la COLA de mensajes.
#include <sys/msg.h> // funciones para enviar recibir mensaje.
#include <string.h> // textazo p 

//Definir la KEY:
#define KEY_COLA 123456

// estructura del mensaje que enviaremos.

struct MiMensaje{

    long mtype;
    char texto[100];
};

// variable globales para poder usarlas tanto en funciones como en el main.
sigjmp_buf salto_sigint;
sigjmp_buf salto_sigusr1;

char * myfifo = "/tmp/fifoLab"; //  aunque yo use --> /tmp/ferfifo, pero en el hipotetico caso de que usted use un script para revisar las tareas,... bueno, ahi esta igual que el doc del lab.
char buf[1000];
int fd;

void handler_sigint(int sigint){
    printf("Llego la senal SIGINT (%d)\n", sigint);
    siglongjmp(salto_sigint, 16);
}

void handler_sigusr1(int sigusr1){
    printf("Llego la senal SIGUSR1(%d)\n", sigusr1);
    siglongjmp(salto_sigusr1, 24);
}

int main(){

    printf("PID: %d\n", getpid());

    // Crear el comunicador con el Kernel y cree la cola (si es primer vez) con esa key:
    int msqid;
    msqid = msgget(KEY_COLA, IPC_CREAT | 0666);

    // alistar el mensaje:
    struct MiMensaje mensaje1; // como objeto

    // Vicular la SENAL que reciba, hacia su HANDLER:
    signal(SIGINT, handler_sigint);
    signal(SIGUSR1, handler_sigusr1);

    // verificar el valor actual del jump:
    if(sigsetjmp(salto_sigint,1) == 16){
        fd = open(myfifo, O_RDONLY);
        read(fd, buf, sizeof(buf));
        printf("Received: %s\n", buf);
        close(fd);
    }

    if(sigsetjmp(salto_sigusr1,1) == 24){
        mensaje1.mtype = 3;
        strcpy(mensaje1.texto, buf);
        msgsnd(msqid, &mensaje1, sizeof(mensaje1.texto), 0);
    }

    while(1){

        printf("Proceso activo...\n");

        sleep(3);
    }

    return 0;
}