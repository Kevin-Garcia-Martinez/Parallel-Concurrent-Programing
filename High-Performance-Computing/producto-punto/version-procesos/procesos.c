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

/* Variables globales, estas no se comparten entre los procesos hijos  */
int producto_punto = 0; /* Incializamos el promedio para que no tenga basura */

/* Exportamos las variables globales creadas en principal.c */
extern int *A, *B; 

/* Procesamiento/Paralelismo a nivel de datos usando bloques 

Se divide el conjunto de datos (arreglo) en bloques. Por bloques
la memoria es continua, por lo que eso facilita que podamos regresar
esos datos que están continuos en memoria por una tubería.

Para el cálculo del producto punto el resultado del algoritmo siempre 
será un sólo número, así que no importa si los datos están consecutivos 
o salteados en memoria, ya que la suma siempre será la misma. 

*/

void proceso_hijo( int np, int pipefd[]){
    /* np será nuestro núcleo ahora y con el determinaremos el inicio y fin de cada bloque
    que tendrá cada proceso, se le llama nucleo porque vamos a trabajar con los núcleos físicos
    del procesador. */
    register int i;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    
    /* Procesamiento de datos de manera ALTERNADA, esta versión es más rápida*/ 
    for ( i = np; i <NUM_DATOS; i+=NUM_PROC)
        producto_punto+= A[i] * B[i];

    /* Le retornamos la dirección de memoria del 'producto punto' que calculo este 
    proceso hijo al proceso padre, para que este pueda acumular las sumas parciales */
    write( pipefd[1], &producto_punto, sizeof(int));
    /* Cerramos el descriptor de escritura porque ya no lo vamos a utilizar */
    close( pipefd[1]);

    /* Retornamos el número de proceso hijo que termino su ejecución, estos procesos hijo ya
     no ejutarán más código apartir de aquí, y entrarán en estado Zombi.
    */
    exit(np);
}


/* Liberaremos a los procesos hijos que se encuentran en estado Zombi*/
void proceso_padre( int pipefd[NUM_PROC][2] ){
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado; 
    /* A través de este identificador sabremos cuál es el número de tubería que debemos leer de acuerdo 
    al número de proceso recuperado con wait */
    int numproc;
    register int np;
    
    int suma_parcial;

    /* El proceso padre se encargará de hacer la suma parcial que cada proceso hijo escribió en su tubería */
    for( np=0; np<NUM_PROC; np++){
        /* EL número de proceso 'numproc' que devuelve wait es el que utilizamos para asignarles 
        tareas a cada uno de los procesos hijo */
        pid = wait(&estado); /* El número de proceso que devuelve el proceso hijo mediante exit() se almacena en 'estado' */
        /* Hacemos el corrimiento para obtener el número de proceso que envío el proceso hijo */
        numproc = estado>>8; 
        /* Cerramos el descriptor de archivo de escritura, ya que no lo utilizamos en el proceso padre */
        close( pipefd[numproc][1] );
        /* Leemos la suma parcial del bloque de datos que procesó cada proceso hijo */
        read( pipefd[numproc][0], &suma_parcial , sizeof(int));
        /* Acumulamos las sumas parciales de cada hijo en la variable 'promedio' del proceso padre */
        producto_punto+=suma_parcial;
        /* Si el proceso que acaba de terminar es el proceso hijo 0 */
        printf("Termino el proceso %d con pid: %d \n", numproc, pid);  
         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );
    }
   

}