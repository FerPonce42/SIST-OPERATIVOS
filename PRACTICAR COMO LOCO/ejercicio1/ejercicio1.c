/*

Escribe un programa en C donde:

El proceso padre crea un pipe.
Hace fork.
El hijo lee del pipe y muestra en pantalla lo que recibió.
El padre escribe un mensaje al pipe.

*/


#include <stdio.h>
#include <unistd.h>


int main(){

    // crear el pipe

    int fd[2];

    pipe(fd);

    char msg_padre[] = "Holi Hijito";

    pid_t pid = fork();


    char msg_hijo[100];


    if(pid == 0){

        read(fd[0], msg_hijo, sizeof(msg_hijo));
        printf("%s\n",msg_hijo);

    }else{


        write(fd[1], msg_padre, sizeof(msg_padre));
    }


}
