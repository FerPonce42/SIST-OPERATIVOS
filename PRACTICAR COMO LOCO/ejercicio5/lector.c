/*
Enunciado de lector.c:

Escribe un programa que:

Abra el FIFO /tmp/FerFifo para lectura (O_RDONLY).
Lea de ahí con read(), guardando el resultado en un buffer.
Imprima el contenido recibido en pantalla.
Cierre el file descriptor.

No necesita mkfifo (el escritor ya lo crea). Escríbelo completo, aplicando el mismo patrón que ya usaste en escritor.c y en el Ejercicio 1 de pipe.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){

    int fd = open("/tmp/FerFifo", O_RDONLY);

    char buffer[100];


    read(fd, buffer, sizeof(buffer));

    printf("%s", buffer);

    close(fd);
}