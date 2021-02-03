/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"
#include "procesamiento.h"

/* Arreglo de los datos */
extern int *datos; 
extern pthread_mutex_t bloqueo;
/* Variables globales */
int res_primos, res_pares, res_promedio;


void * funcion_hilo( void *arg ){
    /* Recuperamos el número de hilo que se esta ejecutando */
    int nh = *(int *)arg;
    printf("Hilo %d en ejecución \n", nh);
	/* Establecemos la zona crítica para los hilos */
	pthread_mutex_lock( &bloqueo );
	/* EL indice me permita asignar tareas a cada uno de los hilos que se esta ejecutando */
	if( nh == 0 )
		res_promedio = obtenerPromedio( datos );
	else if( nh == 1 )
		res_pares = buscarPares( datos );
	else if( nh == 2 )
		ordenarDatos( datos );
	else if( nh == 3 )
        res_primos = contarPrimos( datos );
	pthread_mutex_unlock( &bloqueo );

    /* Retornamos el número de hilo, que es el argumento que recibimos en esta función */
	pthread_exit( arg );
}