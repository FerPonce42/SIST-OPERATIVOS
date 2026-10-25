#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/msg.h>

#include <string.h>
#include <stdlib.h>


#include <signal.h>
#include <setjmp.h>

#define KEY 1234

sigjmp_buf caja;


// crear cola mensaje
int id_key;



// crear tuberia:
int fd[2];

struct Mensaje{

    long m_type;
    char contenido[100];
}; 

void manejador_sigint(int sigint){
    printf("Manejador de SIGINT! ACTIVO\n");




    struct Mensaje m;
    m.m_type = 1;
    strcpy(m.contenido, "Hola soy el Proceso A hablando a B!\n");

    msgsnd(id_key, &m, sizeof(m.contenido), 0);



    siglongjmp(caja, 2);
}

void manejador_sigtrap(int sigtrap){
    printf("Manejador de SIGTRAP! ACTIVO\n");

    char msg_pipe[100] = "Hola soy el Proceso A hablando a C!";

    write(fd[1], msg_pipe, sizeof(msg_pipe));

    siglongjmp(caja, 5);
}

int main(){
printf("YO PROCESO A con PID:%d\n",getpid());
    signal(SIGINT, manejador_sigint);
    signal(SIGTRAP, manejador_sigtrap);
// crear cola mensaje
    id_key = msgget(KEY, IPC_CREAT | 0666);

    // crear pipe
    pipe(fd);


//creacion de B:
    int pid_b = fork();

    if(pid_b == 0){
        // clonacion:
        int pid_c = fork();
        if(pid_c == 0){

            char descriptor[20];

            sprintf(descriptor, "%d", fd[0]);

            char* args[] = {"./proceso_c", descriptor, NULL};

            execv("./proceso_c", args);

            
        }

        struct Mensaje r;

        msgrcv(id_key, &r, sizeof(r.contenido),1,0);

        printf("YO PROCESO B con PID:%d\n",getpid());
        printf("Recibi el sgte mensaje: %s",r.contenido);

        return 0;

    }




// Solo A (el padre) llega aquí:
sigsetjmp(caja, 1);
    while(1){
        pause();
    }

}