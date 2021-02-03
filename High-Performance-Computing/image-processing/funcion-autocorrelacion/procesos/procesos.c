/** @brief procesos.c, es programa gestiona 
 *  las funciones que relizarán los procesos
 *  tanto el padre como los hijos. 
 */ 

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include "defs.h"
/* Exportamos las variables globales creadas en principal.c */
extern float *rxx, *x;


void proceso_padre( int pipefd[NUM_PROC][2] ){
    
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado; 
    /* A través de este identificador sabremos cuál es el número de tubería que debemos leer de acuerdo 
    al número de proceso recuperado con wait */
    int numproc;
    register int np;
    /* Número de elementos por bloque */
    int eleBloque = NUM_DATOS / NUM_PROC, inicio;
    
    for( np=0; np<NUM_PROC; np++){
        /* El número de proceso que devuelve el proceso hijo mediante exit() se almacena en 'estado' al utilizar wait()*/
        pid = wait(&estado); 
        /* Hacemos el corrimiento para obtener el número de proceso que envío el proceso hijo */
        numproc = estado>>8; 
        /* Cerramos el descriptor de archivo de escritura, ya que no lo utilizamos en el proceso padre */
        close( pipefd[numproc][1] );
        /* Se calcula el Inicio de Bloque de acuerdo al número de proceso 'numproc' */
        inicio = numproc*eleBloque;
        /* Leemos el bloque que procesó cada uno de los procesos hijo apartir del Inicio de bloque */
        read( pipefd[numproc][0], rxx+inicio , sizeof(float)*eleBloque );
        printf("Termino el proceso %d con pid: %d \n", numproc, pid);  
         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );
    }

}

/* En los procesos no existen condiciones de carrera, ya que cada proceso
 * trabaja con una copia de los datos de manera independiente. 'np' será 
 * nuestro nuclero ahora y con el se determinará el inicio y el fin de 
 * cada bloque que procesará cada proceso. 
 */
void proceso_hijo( int np, int pipefd[]){
    /* El número de hilo que entrará a ejecutar esta función ahora será el núcleo */
    int nucleo = np ;
    /* Número de elementos por bloque, es decir, el tamaño de bloque */ 
    int elemBloque = NUM_DATOS / NUM_PROC; /* Número de datos dividido por el número de hilos que tenemos */
    /* Calculamos el inicio de bloque, en donde comenzará cada hilo a realizar la multiplicacion del subarreglo */
    int iniBloque = nucleo * elemBloque;
    /* Calculamos el fin de bloque */
    int finBloque = iniBloque + elemBloque;
    /* Vairables para iterar dentro del ciclo */
    register int l;
    register int n;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    

    /* Procesamiento de datos de manera ALTERNADA, esta versión es más rápida*/ 
    for ( l = iniBloque; l < finBloque; l++){
        for ( n=l; n<=NUM_DATOS-1; n++){
            rxx[l] += x[n]*x[n-l];
        }
    }
    
    /* Escribimos la dirección de memoria correspondiente al inicio de bloque que procesó cada uno
    de los procesos en la tubería para mandársela al proceso padre y que este pueda leerla */
    write( pipefd[1], rxx + iniBloque, sizeof(float)*elemBloque);
    /* Cerramos el descriptor de escritura porque ya no lo vamos a utilizar */
    close( pipefd[1]);

    /* Retornamos el número de proceso hijo que termino su ejecución, estos procesos hijo ya
     no ejutarán más código apartir de aquí */
    exit(np);
}




