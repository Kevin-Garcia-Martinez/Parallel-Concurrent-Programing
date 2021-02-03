/* PARALELISMO EN BLOQUES */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
/* Modulos propios */
#include "defs.h"
#include "procesamiento.h"
#include "hilos.h"
#include "helper.h"
#include "archivos.h"

float *A, *B, *C;

int main(){

    /* Register le indica al compilador que le asigne una variable un registro en lugar de una dirección de memoria */
    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];

    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    B = reservarMemoria();
    C = reservarMemoria();
    /* Llenamos los arreglos */
    leerDatosArchivo( A, "pulso" );
    funcionVentanaHan( B );
    /* Imprimimos los arreglos */
    imprimirArreglo( A );
    imprimirArreglo( B );
    /* Guardamos los valores obtenidos en archivos */
    guardaDatosArchivo( A, "sen" );
    guardaDatosArchivo( B, "han" );

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

    imprimirArreglo( C );
    guardaDatosArchivo( C, "res" );
    /* Liberamos la memoria reservada para los arreglos */
    free( A );
    free( B );
    free( C );
    
    return 0;
}