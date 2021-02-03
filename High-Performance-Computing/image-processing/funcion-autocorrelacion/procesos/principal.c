#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include "defs.h"
#include "procesamiento.h"
#include "procesos.h"
#include "helper.h"
#include "archivos.h"

float *rxx, *x;

int main(){
    /* Register le indica al compilador que le asigne una variable un registro en lugar de una dirección de memoria */
    register int np;
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;

    /* Reservamos memoria para los arreglos */
    /* Reservamos memoria para los arreglos */
    x = reservarMemoria();
    rxx = reservarMemoria();
    /* Llenamos los arreglos */
    leerDatosArchivo( x, "res" );
    
    for ( np = 0; np < NUM_PROC; np++)
    {
        /* Se crea un tubería por cada proceso hijo */
        edo_pipe = pipe( &pipefd[np][0] );

        /* Si la tubería no se creo exitosamente */
        if( edo_pipe == -1){
            perror("Error al crear la tubería \n");
            exit(EXIT_FAILURE);
        }
        /* Lanzamos el proceso hijo */
        pid = fork();

        if( pid == -1){
            perror("Error al crear el proceso \n");
            exit(EXIT_FAILURE);
        } 
        /* Si es cero, entonces significa que se creo el proceso hijo exitosamente !0 = 1*/
        if( !pid){
            /* Enviamos el número de proceso que se está ejecutando  y la dirección de 
               su propia tubería */
            proceso_hijo(np, &pipefd[np][0]);
        }
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre(pipefd);
    /* EL proceso padre recibe todo el arreglo de tuberías */
    imprimirArreglo( rxx );
    guardaDatosArchivo( rxx, "auto-core" );
    /* Liberamos la memoria de cada uno de los arreglos */
    free( x );
    free( rxx );
    
    return 0;
}


