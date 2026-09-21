/*
Nivel 2 (4 técnicas) — Pipe + Fork + Dup2 + Execv

El proceso padre crea un pipe y hace fork(). El hijo usa dup2 para que su entrada estándar apunte al extremo de lectura del pipe, 
cierra el fd redundante, y usa execv para convertirse en un segundo programa (que simplemente lee de STDIN y muestra lo recibido). 
El padre escribe un mensaje al pipe.

*/