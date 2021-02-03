/** @brief principal.c, este programa realiza 
 *  la ejecución de los procesos hijo
 */ 

#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<unistd.h>
/* bibliotecas donde se encuentran definidas las funciones */
#include "procesos.h"
#include "procesamiento.h"
#include "helper.h"
#include "defs.h"

/* Cada vez que corremos este programa, el Sistema Operativo
   le asigna un valor distinto al pid del proceso padre.*/
int main(){
    register int np;
    int *datos; /* Arreglo de los datos */
    pid_t pid;
    /* Reservamos memoria para el arreglo de datos */ 
    datos = reservarMemoria();
    /* Llenamos el arreglo con números pseudoaleatorios mediante un paso por referencia */
    llenarArreglo( datos ); /* Paso por referencia del arreglo al cual le reservamos memoria*/
    imprimirArreglo( datos );
    printf("\n\n");
    /* Generamos los procesos hijo */
    for (np = 0; np < NUM_PROC; np++)
    {
        /* Cremamos un proceso hijo */
        pid = fork();
        if( pid == -1){
            perror("Error al crear el proceso \n");
            exit(EXIT_FAILURE);
        } 
        /* Código del proceso hijo */
        /* La función fork asigna a pid = '1' si se trata del proceso padre y '0' si se trata del proceso hijo*/
        if( !pid){
            /* Le pasamos el número de proceso hijo que se esta ejecutando */
            proceso_hijo(np, datos);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre();
    /* Liberamos la memoria asignada */
    free(datos);

    return 0;
}