/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

/* Exportamos las variables globales creadas en principal.c */
extern float *rxx, *x;

void * funcion_hilo( void *arg ){
    /* El número de hilo que entrará a ejecutar esta función ahora será el núcleo */
    int nucleo = *(int *)arg;
    /* Número de elementos por bloque, es decir, el tamaño de bloque */ 
    int elemBloque = NUM_DATOS / NUM_HILOS; /* Número de datos dividido por el número de hilos que tenemos */
    /* Calculamos el inicio de bloque, en donde comenzará cada hilo a realizar la multiplicacion del subarreglo */
    int iniBloque = nucleo * elemBloque;
    /* Calculamos el fin de bloque */
    int finBloque = iniBloque + elemBloque;
    /* Variable del ciclo for */
    register int l;
    register int n;
     
    printf("Hilo %d en ejecución \n", nucleo);
    /* Se realiza la paralelización a nivel de datos, ahora el ciclo for se ejecuta de manera paralela */
    /* Paralelización a nivel de datos: Forma Alternada */
    for( l=iniBloque; l<finBloque; l++ ){
        for ( n=l; n<=NUM_DATOS-1; n++){
            rxx[l] += x[n]*x[n-l];
        }
    }

    /* Retornamos el número de hilo, que es el argumento que recibimos en esta función */
	pthread_exit( arg );
}