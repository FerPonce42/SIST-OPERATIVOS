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

        char fd_str[10];
        sprintf(fd_str, "%d", pipefd[0]);

        execlp("./receptor", "./receptor", fd_str, NULL);

        perror("Error en exec");
        exit(1);
    } else {
        close(pipefd[0]);

        char mensaje[] = "Mensaje enviado a Fernando desde el Padre por Pipe\n";
        write(pipefd[1], mensaje, sizeof(mensaje));

        close(pipefd[1]);
        wait(NULL);
    }

    return 0;
}