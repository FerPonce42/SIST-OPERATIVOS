/*
Nivel 3 (7 técnicas) — Integrador completo

Un Proceso A espera una señal. Al recibirla (signaling + sigsetjmp/siglongjmp), hace fork(): el hijo usa dup2 + 
execv (pipe + fork + dup2 + execv) para convertirse en un Proceso B. El Proceso B, tras recibir el dato por el pipe, lo 
reescribe hacia un FIFO. Un tercer proceso lee ese FIFO y lo reenvía por Message Queue a un cuarto proceso, 
que lo recibe y lo imprime en pantalla.

*/