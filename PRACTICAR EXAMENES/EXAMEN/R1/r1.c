/*
Nivel 1 (3 técnicas) — Signaling + Message Passing + Fork

Un proceso espera una señal (SIGUSR1/SIGUSR2/etc). Al recibirla, usa sigsetjmp/siglongjmp para volver al punto de control, 
hace fork(), y el hijo manda por Message Queue un mensaje indicando qué señal llegó (mtype = tipo de señal). 
El padre sigue esperando nuevas señales.
*/