/** @brief defs.h, Archivo de definiciones
 *  del programa.
 */

#ifndef DEFS_H
#define DEFS_H

/* Número de hilos para paralelizar el algoritmo */
#define NUM_HILOS 4

/* El tamaño del kernel será de 3x3 debido a que utilizaremos
como máscara el gradiente fila y el gradiente columna. */
#define DIMASK 3

/* Definimos un nuevo tipo de dato de tipo estructura que se utilizará
   para enviar más de un parametro a la función del hilo */
typedef struct parameters {
    unsigned char *imagenGray;
    unsigned char *imagenSegmentada;
    uint32_t width; 
    uint32_t height;
} parameters;

#endif