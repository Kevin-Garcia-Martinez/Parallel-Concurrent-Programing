/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

/* Exportamos las variables globales creadas en principal.c */
extern float *A, *B, *C, *D;
extern int flag;

void * funcion_hilo( void *arg ){
    /* Número de hilo que entra a ejecutar esta función (nucleo)*/
    int nh = *(int *)arg;
    /* Variable del ciclo for */
    register int i, l, n;

    printf("Hilo %d en ejecución \n", nh);

    if( flag == 0 ){
        /* Se realiza la paralelización a nivel de datos, ahora el ciclo for se ejecuta de manera paralela */
        for( i = nh; i<NUM_DATOS; i += NUM_HILOS )
        {
            C[i] = A[i] * B[i];
        }
    }

    else{
        
        for ( l = nh; l < NUM_DATOS; l += NUM_HILOS )
        {
            for ( n = l; n <= NUM_DATOS-1; n++ )
            {
                D[l] += C[n] * C[n-l];
            }
        }
    }

    /* Retornamos el número de hilo, que es el argumento que recibimos en esta función */
	pthread_exit( arg );
}