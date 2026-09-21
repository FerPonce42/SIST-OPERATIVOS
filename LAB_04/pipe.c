#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    int     fd[2];
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];

    // 1. Crear el pipe antes del fork para que ambos procesos lo compartan
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    // 2. Duplicar el proceso
    if ((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    // Proceso Hijo (Escritor)
    if (childpid == 0) {
        close(fd[0]); // Cierra la lectura, el hijo solo va a escribir
        
        write(fd[1], string, (strlen(string) + 1));
        exit(0);
    }
    // Proceso Padre (Lector)
    else {
        close(fd[1]); // Cierra la escritura, el padre solo va a leer
        
        read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s", readbuffer);
    }

    return 0;
}
*/ 
=============================================================================
GUÍA PRÁCTICA DE TERMINAL: COMPRENDIENDO FORK() Y PIPES
=============================================================================

El objetivo de tu profesor con este código es que entiendas tres conceptos clave del sistema operativo:
1. Concurrencia: Dos procesos (padre e hijo) ejecutándose al mismo tiempo de forma independiente.
2. Comunicación Interprocesos (IPC): Cómo comparten datos a través de una tubería (pipe).
3. Sincronización por bloqueo: El proceso que lee se "congela" automáticamente hasta que el otro escribe datos.

Para ver esto en acción en tu terminal, sigue estos pasos:

-----------------------------------------------------------------------------
PASO 1: COMPILACIÓN LIMPIA
-----------------------------------------------------------------------------
Guarda el código en un archivo llamado "pipe_test.c" y compílalo usando el siguiente comando:

gcc -Wall pipe_test.c -o pipe_test

* Nota: El parámetro "-Wall" le pide al compilador que muestre todas las advertencias si encuentra código peligroso o variables sin usar.

-----------------------------------------------------------------------------
PASO 2: EJECUCIÓN BÁSICA (COMPRENDER EL FLUJO DEL PIPE)
-----------------------------------------------------------------------------
Ejecuta el programa de forma normal:

./pipe_test

* Qué observar: El programa parece instantáneo. El hijo escribe "Hello, world!" en el extremo de escritura del pipe (fd[1]) y el padre lo lee de inmediato desde el extremo de lectura (fd[0]). El sistema operativo coordina el paso de datos en memoria sin necesidad de archivos en el disco duro.

-----------------------------------------------------------------------------
PASO 3: EL EXPERIMENTO CLAVE (COMPRENDER LA SINCRONIZACIÓN Y EL BLOQUEO)
-----------------------------------------------------------------------------
Para experimentar cómo se coordinan, modifica temporalmente tu código añadiendo un "sleep" o un "getchar" en el proceso hijo justo ANTES del "write", de esta forma:

if(childpid == 0) {
    close(fd[0]);
    printf("[Hijo] Voy a dormir 5 segundos antes de escribir...\n");
    sleep(5); 
    write(fd[1], string, (strlen(string)+1));
    exit(0);
}

Vuelve a compilar y ejecútalo con: ./pipe_test

* Qué observar en la terminal: Verás que el texto "Received string:" NO aparece de inmediato. El padre se queda completamente congelado (bloqueado) en la línea del "read()". El sistema operativo duerme al proceso padre para no gastar CPU, y lo despierta automáticamente en el segundo 5, justo cuando el hijo termina su sleep y escribe en el pipe. Esta es la magia de la sincronización de pipes.

-----------------------------------------------------------------------------
PASO 4: VER LOS DOS PROCESOS EN EL SISTEMA OPERATIVO
-----------------------------------------------------------------------------
Si quieres comprobar que realmente existen dos procesos separados en la memoria de tu computadora, añade un "sleep(30)" al final de los bloques del padre y del hijo para darte tiempo de verlos:

1. Ejecuta el programa en tu terminal principal:
   ./pipe_test

2. Mientras el programa está corriendo (y antes de que pasen los 30 segundos), abre otra pestaña o ventana de la terminal y ejecuta:
   ps -ef | grep pipe_test

* Qué observar en la terminal: Verás dos líneas de salida con el nombre "pipe_test". Una de ellas mostrará el PID del padre y la otra mostrará el PID del hijo, demostrando que fork() duplicó el programa con éxito en el sistema.
=============================================================================
*/
