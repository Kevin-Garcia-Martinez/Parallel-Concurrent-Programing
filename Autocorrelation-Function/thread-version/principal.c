/* PARALELISMO FORMA ALTERNADA */
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

float *A, *B, *C, *D;
int flag;

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
    D = reservarMemoria();

    /* Leemos los datos del Pulso Cardiaco */
    readDataFromFile( A, "pulso" );
    /* Guardamos la función Hann en un archivo */
    generateHannFunction( B );
    storeDataInFile( B, "Hann" );
    /* Inicializamos la bandera */
    flag = 0;

    /* Procesamiento de la función de Ventaneo */
    for (nh = 0; nh < NUM_HILOS; nh++){  
        nhs[nh] = nh;
        pthread_create( &tids[nh], NULL, funcion_hilo, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&hilo);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El hilo termino su ejecución %d \n", *hilo);
    }

    /* Actualizamos el valor de la bandera */
    flag = 1;
    printf("\n\n");

    /* Procesamiento de la función de Correlación */
    for (nh = 0; nh < NUM_HILOS; nh++){  
        nhs[nh] = nh;
        pthread_create( &tids[nh], NULL, funcion_hilo, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&hilo);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El hilo termino su ejecución %d \n", *hilo);
    }
    
    /* Guardamos el resultado del proceso de ventaneo */
    storeDataInFile( C, "Ventaneo" );
    /* Guardamos el resultado de la Función de Autocorrelación */
    storeDataInFile( D, "Correlacion" );

    /* Liberamos la memoria de cada uno de los arreglos */
    free( A );
    free( B );
    free( C );
    free( D );
    
    return 0;
}


