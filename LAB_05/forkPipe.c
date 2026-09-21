#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    pid_t pid;

    // 1. Crear el pipe (0 lectura, 1 escritura)
    if (pipe(pipefd) == -1) {
        perror("Error en pipe");
        exit(1);
    }

    pid = fork(); // 2. Crear al hijo

    if (pid == 0) {
        // --- HIJO: Va a leer del pipe desde su STDIN ---
        close(pipefd[1]); // Cierra extremo de escritura

        // CONECTAR: Redirige STDIN (0) al extremo de LECTURA del pipe (pipefd[0])
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]); // Ya duplicado, se cierra el original

        // REEMPLAZAR HIJO: Ahora ejecuta "cat" (que lee de STDIN, o sea, del pipe)
        execlp("cat", "cat", NULL);

        perror("Error en exec");
        exit(1);

    } else {
        // --- PADRE: Va a escribir en el pipe ---
        close(pipefd[0]); // Cierra extremo de lectura

        // El padre manda un mensaje al pipe
        char mensaje[] = "¡Hola Hijo! Mensaje enviado desde el pipe por el Padre.\n";
        write(pipefd[1], mensaje, sizeof(mensaje));

        close(pipefd[1]); // Importante cerrar para que el hijo sepa que terminó de escribir
        wait(NULL);       // Esperar a que el hijo termine
    }

    return 0;
}// fior pipo capture son 7 tenicas a implementar y tienes que hacer dentro de la hora dentro de las dos horas. en una clase.. el hciho PROX 

/*
PROX SEMANA CONTROL LABORATORIO

ni idea los temas o tenicaim pero el ahora mismo dice algo de nueva nevatecnica

excevcv() para no se que, algun notiene pipe no se que diablos adsdadada


*/