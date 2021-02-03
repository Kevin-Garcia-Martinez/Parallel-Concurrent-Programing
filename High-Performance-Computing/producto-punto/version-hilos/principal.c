/* Calcular promedio haciendo Paralelismo a nivel de datos */
/* PARALELISMO EN FORMA ALTERNADA */
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "defs.h"
#include "helper.h"
#include "hilos.h"

/* Cálculo del promedio de un bloque de datos */
int *A, *B; 
/* Declaramos el mutex */
pthread_mutex_t bloqueo;
/* Exportamos las variables globales creadas en hilos.c */
extern int producto_punto;

int main(){
    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    B = reservarMemoria();
    /* Llenamos los arreglos */
    llenarArreglo( A );
    llenarArreglo( B );
    /* Imprimimos los arreglos */
    printf("\n\n COMIENZO DEL ARREGLO A \n\n");
    imprimirArreglo( A );
    printf("\n\n COMIENZO DEL ARREGLO B \n\n");
    imprimirArreglo( B );
    /* Incializamos el objeto de sincronización*/
    pthread_mutex_init(&bloqueo, NULL);

    /* En los hilos la memoria es compartida */
    for (nh = 0; nh < NUM_HILOS; nh++){  
        nhs[nh] = nh;
        pthread_create( &tids[nh], NULL, funcion_hilo, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&hilo);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El hilo termino su ejecución %d \n", *hilo);
    }
    /* Imprimimos el producto punto de los Arreglos A y B */
    printf("\n\nEl producto punto es: %d \n", producto_punto);
    /* Liberamos la memoria reservada para los arreglos */
    free( A );
    free( B );
    /* Liberamos el mutex */
    pthread_mutex_destroy(&bloqueo);
    return 0;
}

