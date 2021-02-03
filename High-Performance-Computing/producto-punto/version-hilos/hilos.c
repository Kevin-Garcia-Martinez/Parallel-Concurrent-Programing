/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

/* Variables globales */
int producto_punto = 0; /* Lo inicializamos para que no tenga basura */

/* Exportamos las variables globales creadas en principal.c */
extern int *A, *B; 
extern pthread_mutex_t bloqueo;

void * funcion_hilo( void *arg ){

    /* El número de hilo que entrará a ejecutar esta función ahora será el núcleo */
    int nucleo = *(int *)arg;
    /* Número de elementos por bloque, es decir, el tamaño de bloque */ 
    int elemBloque = NUM_DATOS / NUM_HILOS; /* Número de datos dividido por el número de hilos que tenemos */
    /* Calculamos el inicio de bloque, en donde comenzará cada hilo a realizar la multiplicacion del subarreglo */
    int iniBloque = nucleo * elemBloque;
    /* Calculamos el fin de bloque */
    int finBloque = iniBloque + elemBloque;
    /* Variable del ciclo for */
    register int i;
    /* Esta suma parcial se creará por cada hilo que entre a la función del hilo*/
    int suma_parcial; /* Las variables locales no son compartidas por los hilos */
    /* Como promedio es una variable global, vamos a tener un problema de condición de carrera
    por lo que necesitamos utilizar un mutex para evitar que los hilos accedan al mismo tiempo
    a la variable global 'promedio' */
    printf("Hilo %d en ejecución \n", nucleo);
    suma_parcial = 0;
    
    /* Paralelización a nivel de datos: Forma Alternada */
    for( i=iniBloque; i<finBloque; i++ )
    {
        /* Cada hilo procesa en paralelo la suma parcial del bloque que le fue asignado */
        suma_parcial += A[i] * B[i];
    }

    /*                                  SECCIÓN CRÍTICA
    Hay que añadir la suma parcial al promedio, por lo que el promedio será la única variable
    que se debe proteger con mutex, de este modo el proceso de suma se realiza de manera paralela
    y cada vez que un hilo termine de realizar la suma de su bloque la deberá acumular en 'promedio'
    pero uno a la vez, es decir, es proceso se realiza de manera secuencial. */

    pthread_mutex_lock(&bloqueo);
    producto_punto += suma_parcial; 
    pthread_mutex_unlock(&bloqueo);
    
    /* Retornaremos el resultado de la operación que realizó cada uno de los hilos */
    pthread_exit( arg );
}