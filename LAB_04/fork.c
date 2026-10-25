#include <stdio.h>
#include <stdlib.h> // Necesario para system()
#include <unistd.h> // Necesario para fork() y sleep()

int main() {
    int pid; 
    
    // Al hacer fork, el proceso se duplica.
    pid = fork(); 

    // Si fork() devuelve -1, significa que hubo un error al crear el proceso
    if (pid < 0) {
        perror("Error al hacer fork");
        return 1;
    }

    // Proceso hijo: fork() devuelve exactamente 0
    if (pid == 0) {
        printf("[Hijo] Mi PID real es: %d\n", getpid());
        system("echo '[Hijo Shell] Ejecutando echo desde system...'");
        sleep(10); // Reducido a 10 segundos para la prueba
    } 
    // Proceso padre: fork() devuelve el PID del hijo (un número mayor a 0)
    else {
        printf("[Padre] Mi PID es: %d, y el PID de mi hijo es: %d\n", getpid(), pid);
        system("echo '[Padre Shell] Ejecutando echo desde system...'");
        sleep(10);
    }

    return 0;
}


// execl("/home/julio/P1.exe"," /home/julio/P1.exe",)