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

float *rxx, *x;

int main(){

    /* Register le indica al compilador que le asigne una variable un registro en lugar de una dirección de memoria */
    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];

    /* Reservamos memoria para los arreglos */
    x = reservarMemoria();
    rxx = reservarMemoria();
    /* Llenamos los arreglos */
    leerDatosArchivo( x, "res" );

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

    imprimirArreglo( rxx );
    guardaDatosArchivo( rxx, "auto-core" );
    /* Liberamos la memoria reservada para los arreglos */
    free( x );
    free( rxx );
    
    return 0;
}