/** @brief principal.c, este programa realiza 
 *  la ejecución de cada uno de los hilos.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/* Bibliotecas donde se encuentran definidas las funciones */
#include "hilos.h"
#include "procesamiento.h"
#include "helper.h"
#include "defs.h"

/* Variables globales */ 

int *datos; /* Arreglo de los datos */
extern int res_primos, res_pares, res_promedio;
pthread_mutex_t bloqueo;

int main(){
    
    register int nh = 0;
	pthread_t tids[NUM_HILOS];
	int *hilo, nhs[NUM_HILOS];
    /* Inicializamos el mutex de manera dinámica */
    pthread_mutex_init( &bloqueo, NULL );

    /* Reservamos memoria para el arreglo de datos */ 
    datos = reservarMemoria();
    /* Llenamos el arreglo con números pseudoaleatorios mediante un paso por referencia */
    llenarArreglo( datos ); 
    /* Imprimimos el arreglo original */ 
    imprimirArreglo( datos );

    for( nh = 0; nh < NUM_HILOS; nh++ )
	{
		nhs[nh] = nh;
		pthread_create( &tids[nh] , NULL, funcion_hilo, (void *)&nhs[nh] );
	}

    for( nh = 0; nh < NUM_HILOS; nh++ )
	{
		/* Identificamos el número de hilo que termino de ejecutarse */
		pthread_join( tids[nh], (void **)&hilo );
		/* Imprimimos los resultados de la función del hilo */
        if (*hilo == 0){
            printf("\nHilo %d ejecutándose... Promedio: %d \n\n", *hilo, res_promedio);
        }
        else if(*hilo == 1)
            printf("\nHilo %d ejecutándose... Número de Pares: %d \n\n", *hilo, res_pares);
        else if(*hilo == 2)
        {
            printf("\nHilo %d ejecutándose... Arreglo Ordenado: \n\n", *hilo);
            imprimirArreglo( datos );
        }
        else if(*hilo == 3)
            printf("\nHilo %d ejecutándose... Número de Primos: %d \n\n", *hilo, res_primos);
	}
    
    pthread_mutex_destroy( &bloqueo );

    return 0;
}