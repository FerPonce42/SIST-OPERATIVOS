/*
Vamos con Signaling.

Vas a escribir un programa que:

Registra un handler para SIGUSR1.
Usa sigsetjmp/siglongjmp para volver a un punto específico cuando llega la señal.
Se queda en un loop infinito, mostrando su PID y esperando.

Paso 1 (solo esto por ahora): escribe los #include que necesites, y dentro de main(), 
solo el printf que muestre el PID del proceso (getpid()). No escribas nada de señales todavía.
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

sigjmp_buf pc;

void handler_sigurs1(int sirgusr1){

    printf("Hemos llegado al manejador con: %d", sirgusr1);

    siglongjmp(pc, 42);

}


int main(){

    signal(SIGUSR1, handler_sigurs1);


    pid_t pid = getpid();
    
    if(sigsetjmp(pc, 1) == 0){

    }else{

        printf("Hemos vuelto al main!");

    }


    while(1){

        printf("%d\n", pid);

        sleep(5);

    }


}