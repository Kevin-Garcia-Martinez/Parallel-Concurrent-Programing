
/** @brief procesos.c, es programa gestiona 
 *  las funciones que relizarán los procesos
 *  tanto el padre como los hijos. 
 */ 

#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "defs.h"
#include "procesamiento.h"
#include "helper.h"


void proceso_padre( int pipefd[NUM_PROC][2]){
    
    register int np;
    pid_t pid;
    int estado; 
    int numproc;
    int resultado;
    int res[N];
    
    for( np=0; np<NUM_PROC; np++){
        
        pid = wait(&estado);
        numproc = estado>>8; 
        /* Cerramos el descriptor de archivo de escritura */
        close( pipefd[numproc][1] );
        

        /* Si el proceso que acaba de terminar es el proceso hijo 0 */
        if( numproc == 0){
            /* Leemos el valor que puso el proceso hijo en la tubería */ 
            read( pipefd[numproc][0], res, sizeof(int)*N);
            printf("\nTermino el proceso %d con pid: %d \n", numproc, pid);
            printf("\nEl arreglo ordenado es: \n");
            imprimirArreglo(res);
        }

        else if( numproc == 1){
            /* Leemos el valor que puso el proceso hijo en la tubería */ 
            read( pipefd[numproc][0], res, sizeof(int)*N);
            printf("\nTermino el proceso %d con pid: %d \n", numproc, pid);
            printf("\nEl arreglo modificado es : \n");
            imprimirArreglo(res);
        }

        else if( numproc == 2){
            /* Leemos el valor que puso el proceso hijo en la tubería */ 
            read( pipefd[numproc][0], &resultado, sizeof(int) );
            printf("Termino el proceso %d con pid: %d \n\n", numproc, pid);
            printf("El promedio es: %d \n", resultado);
        }

        else if( numproc == 3){
            /* Leemos el valor que puso el proceso hijo en la tubería */ 
            read( pipefd[numproc][0], &resultado, sizeof(int) );
            printf("Termino el proceso %d con pid: %d \n\n", numproc, pid);
            printf("El número de pares es: %d \n", resultado);
        }

         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );   
    }

}

void proceso_hijo( int np, int pipefd[], int *datos ){

    int pares, promedio;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    /* Con el número de proceso 'np', podemos asignar tareas a los procesos hijos que creamos en el proceso padre*/
    if(np==0){
        ordenarDatos( datos );
        //imprimirArreglo(datos);
        /* Escribimos el resultado en la tubería, enviamos un apuntador */
        write(pipefd[1], datos, sizeof(int)*N);
    }
    /* Si se trata del proceso 1 realiza la multiplicación por una constante */
    else if(np==1){
        multiplicarConstante( datos );
        //imprimirArreglo(datos);
        write(pipefd[1], datos, sizeof(int)*N);
    }

    else if(np==2){
        promedio = obtenerPromedio( datos );
        write(pipefd[1], &promedio, sizeof(int));
   }
    else if(np==3){
        pares = buscarPares( datos );
        write(pipefd[1], &pares ,sizeof(int));
    }


    /* Cerramos el descriptor de escritura porque ya no lo vamos a utilizar */
    close( pipefd[1]);
    /* Retornamos el número de proceso hijo que termino su ejecución */
    exit(np);

}