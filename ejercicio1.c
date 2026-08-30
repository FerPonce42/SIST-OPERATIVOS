/*

Fernando Ponce
Se deberán implementar:
● 3 señales diferentes.
● 3 signal handlers.
● 3 puntos de recuperación mediante sigsetjmp().
● 3 saltos no locales mediante siglongjmp().
3. Objetivos específicos
Al finalizar el laboratorio, el estudiante será capaz de:
1. Identificar el funcionamiento del sistema de señales en entornos Unix/Linux.
2. Registrar y configurar diferentes signal handlers utilizando <signal.h>.
3. Utilizar sigsetjmp() para establecer puntos de recuperación del flujo de ejecución.
4. Utilizar siglongjmp() para realizar saltos no locales desde un signal handler.
5. Diferenciar entre el flujo normal de ejecución y el flujo provocado por una señal.
6. Implementar tres mecanismos independientes de recuperación asociados a diferentes señales.
7. Enviar señales a un proceso mediante el comando kill.
8. Utilizar Ctrl+C para generar SIGINT.
9. Mantener el proceso activo después de recibir y procesar cada señal
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
// variable globales para poder usarlas tanto en funciones como en el main.
sigjmp_buf salto_sigint;
sigjmp_buf salto_sigusr1;
sigjmp_buf salto_sigusr2;


void handler_sigint(int sigint){
    printf("Llego la senal SIGINT\n");
    siglongjmp(salto_sigint, 16);
}

void handler_sigusr1(int sigusr1){
    printf("Llego la senal SIGUSR1\n");
    siglongjmp(salto_sigusr1, 24);
}

void handler_sigusr2(int sigusr2){
    printf("Llego la senal SIGUSR2\n");
    siglongjmp(salto_sigusr2, 42);
}





int main(){

    printf("PID: %d\n", getpid());

    // Vicular la SENAL que reciba, hacia su HANDLER:
    signal(SIGINT, handler_sigint);
    signal(SIGUSR1, handler_sigusr1);
    signal(SIGUSR2, handler_sigusr2);


    // verificar el valor actual del jump:
    if(sigsetjmp(salto_sigint,1) == 16){
        printf("La senal INT volvio al main, regreso al bucle!\n");
    }
    
    if( sigsetjmp(salto_sigusr1,1) == 24 ){
        printf("La senal URS1 volvio al main, regreso al bucle!\n");
    }
    
    if( sigsetjmp(salto_sigusr2,1) == 42 ){
        printf("La senal URS2 volvio al main, regreso al bucle!\n");
    }



    while(1){

        printf("Proceso activo...\n");

        sleep(3);
    }


}