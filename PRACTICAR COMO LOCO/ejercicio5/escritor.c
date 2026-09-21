/*
Vamos entonces con escritor.c primero.

Escritor.c — debe:

Crear el FIFO con mkfifo (igual que ya hiciste).
Abrir el FIFO para escritura con open("/tmp/FerFifo", O_WRONLY).
Escribir un mensaje con write().
Cerrar con close().
*/


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){

    mkfifo("/tmp/FerFifo", 0666);


    int fd = open("/tmp/FerFifo", O_WRONLY);


    char m[100] = "Mensaje de escritor!!!";

    write(fd, m, sizeof(m));

    close(fd);
}