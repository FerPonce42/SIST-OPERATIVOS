#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("Error en pipe");
        exit(1);
    }

    pid = fork();

    if (pid == 0) {
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execlp("cat", "cat", NULL);

        perror("Error en exec");
        exit(1);
    } else {
        close(pipefd[0]);

        char mensaje[] = "¡Hola Hijo! Mensaje enviado desde el pipe por el Padre.\n";
        write(pipefd[1], mensaje, sizeof(mensaje));

        close(pipefd[1]);
        wait(NULL);
    }

    return 0;
}