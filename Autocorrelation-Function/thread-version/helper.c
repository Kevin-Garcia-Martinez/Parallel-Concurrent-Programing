#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "defs.h"


void imprimirArreglo( float *datos ){
    register int i;
    for ( i = 0; i < NUM_DATOS; i++){
        if(!(i%16)){
            printf("\n");
        }

        printf("%.4f ", datos[i]);
    }
    printf("\n");
    
}

/* Devuelve el número de bytes reservados en memoria */
float *reservarMemoria(){
    float * mem;
    /* malloc reserva una cantidad de bytes y devuelve un apuntador de tipo void*/
    mem = (float *) malloc( NUM_DATOS*sizeof(float) );
    /* Retornamos la memoria si es que se asignó correctamente*/
    if(!mem){ /* si la memoria no se asigno 'null'*/
        perror("Error al asignar memoria \n");
        exit(EXIT_FAILURE);
    }

    return mem;
}