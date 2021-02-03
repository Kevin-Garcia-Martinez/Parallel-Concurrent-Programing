#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

#include "defs.h"
#include "helper.h"
#include "procesos.h"

/* Variables Globales */
int *A, *B; 

/* Exportamos las variables globales creadas en procesos.c */
extern int producto_punto;

int main(){
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;
    register int np;
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
    
    /* Generamos los procesos hijos */
    for (np = 0; np < NUM_PROC; np++)
    {
        /* Se crea un tubería por cada proceso hijo */
        edo_pipe = pipe( &pipefd[np][0] );

        /* Si la tubería no se creo exitosamente */
        if( edo_pipe == -1){
            perror("Error al crear la tubería \n");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if( pid == -1){
            perror("Error al crear el proceso \n");
            exit(EXIT_FAILURE);
        } 
        
        if( !pid){
            /* Enviamos el número de proceso que se está ejecutando  y la dirección de 
               su propia tubería */
            proceso_hijo(np, &pipefd[np][0]);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre(pipefd);
    /* EL proceso padre recibe todo el arreglo de tuberías */
     
    /* Imprimimos el producto punto */
    printf("\n\nEl producto punto es: %d\n", producto_punto);
    
    /* Liberamos la memoria de cada uno de los arreglos */
    free( A );
    free( B );

    return 0;
}


