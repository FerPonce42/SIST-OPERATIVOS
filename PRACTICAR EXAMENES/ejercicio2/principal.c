/*
Ejercicio 2: Escribe DOS archivos:

programa2.c — un programa que simplemente lea de STDIN_FILENO (como si fuera teclado normal) y muestre lo que recibió.
principal.c — el proceso padre crea un pipe, hace fork; el hijo debe hacer dup2 para que su entrada estándar apunte al pipe,
 cerrar lo que no necesite, y luego usar execv para convertirse en programa2; el padre escribe un mensaje al pipe.

Este es exactamente el patrón que ya viste en tu código original (el de dup2 que me compartiste), pero ahora quiero que TÚ lo reconstruyas desde cero, 
sin mirar el original. Empieza por programa2.c — es el más simple. Adelante.

*/


#include <stdio.h>
#include <unistd.h>

int main(){

    int fd[2];

    pipe(fd);

    pid_t pid = fork();


    if(pid == 0){

    //el hijo debe hacer dup2 para que su entrada estándar apunte al pipe,   
        dup2(fd[0], 0);

        close(fd[0]);


        char* arg[] = {"./programa2", NULL};

        execv("./programa2", arg);


    }else{

        char msg[100] = "Vas a caer hdp";

        write(fd[1], msg, sizeof(msg));

    }


}