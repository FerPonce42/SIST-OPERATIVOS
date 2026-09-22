#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sys/msg.h>

#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <setjmp.h>


int main(int argc, char* args[]){

    int fd_nuevo = atoi(args[1]);

    char buffer[100];

    read(fd_nuevo, buffer, sizeof(buffer));


    printf("YO PROCESO C con PID:%d\n",getpid());
    printf("Recibi el sgte mensaje: %s",buffer);    



}