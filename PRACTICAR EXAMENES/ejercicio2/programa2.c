/*
programa2.c — un programa que simplemente lea de STDIN_FILENO (como si fuera teclado normal) y muestre lo que recibió.

*/

#include <stdio.h>
#include <unistd.h>

int main(){

    char buffer[100];

    read(0, buffer, sizeof(buffer));

    printf("%s\n", buffer);

}