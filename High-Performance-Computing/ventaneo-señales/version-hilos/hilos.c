/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

/* Exportamos las variables globales creadas en principal.c */
extern float *A, *B, *C;

void * funcion_hilo( void *arg ){
    /* Número de hilo que entra a ejecutar esta función (nucleo)*/
    int nucleo = *(int *)arg;
    /* Variable del ciclo for */
    register int i;

    printf("Hilo %d en ejecución \n", nucleo);
    /* Se realiza la paralelización a nivel de datos, ahora el ciclo for se ejecuta de manera paralela */
    for( i=nucleo; i<NUM_DATOS; i+= NUM_HILOS )
    {
        /* No hay problemas de condición de carrera, debido
        a que los indices son distintos para cada hilo que entrará
        a ejecutar la función, ya que nucleo va a ser igual al identificador
        del hilo que ejecutará esta función */
        C[i] = A[i] * B[i];
    }

    /* Retornamos el número de hilo, que es el argumento que recibimos en esta función */
	pthread_exit( arg );
}