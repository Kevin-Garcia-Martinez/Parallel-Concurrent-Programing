
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


void proceso_padre( ){
    register int np;
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado, retorno;
    /* Pasamos a wait una dirección de memoria para que en esta se guarde el valor que 
    regreso el proceso hijo en la función exit() */
    for( np=0; np<NUM_PROC; np++){
        pid = wait(&estado);
        retorno = estado>>8;
        if(np == 0){
            printf("Proceso hijo con pid: %d y Numero Mayor: %d\n", pid, retorno);
        } else if (np == 1) {
            printf("Proceso hijo con pid: %d y Numero Menor: %d\n", pid, retorno);
        }else if (np == 2){
            printf("Proceso hijo con pid: %d y Numero Promedio: %d\n", pid, retorno);
        }else if (np == 3){
            printf("Proceso hijo con pid: %d y cantidad numeros pares: %d\n", pid, retorno);
        }
        
    }

}

void proceso_hijo( int np, int *datos){
    int mayor, menor, pares, promedio;
    /* Con el número de proceso 'np' podemos asignar tareas a cada uno de los 
       procesos hijo que hemos creado con fork() desde el proceso padre. */
    if(np==0){
        mayor = buscarMayor( datos );
        /* Con la función exit() le retornaremos al proceso padre el valor que calculó
        el proceso hijo */
        exit(mayor); 
    }
     
    else if(np==1){
        menor = buscarMenor( datos );
        /* Retornamos al proceso padre el número menor de nuestro arreglo de datos. Al
        utilizar exit() el proceso hijo ya no sale de aquí, termina su ejecución por completo */
        exit(menor);
    }

    else if(np==2){
        promedio = obtenerPromedio( datos );
        exit(promedio);
    }

    else if(np==3){
        pares = buscarPares( datos );
        exit(pares);
    }

}