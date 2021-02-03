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
    int *datos; /* Arreglo de los datos */
    /* Reservamos memoria para el arreglo de datos */ 
    datos = reservarMemoria();
    /* Llenamos el arreglo con números pseudoaleatorios mediante un paso por referencia */
    llenarArreglo( datos ); /* Paso por referencia del arreglo al cual le reservamos memoria*/
    imprimirArreglo( datos );
    printf("\n\n");
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;
    register int np;
    
    /* Generamos los procesos hijos */
    for (np = 0; np < NUM_PROC; np++)
    {
        /* Para crear una tubería necesitamos de un arreglo para los descriptores de archivo 'pipefd[2]'
        pero como hemos creado una matriz, ahora enviaremos la dirección del primer elemento de cada uno
        de los arreglos de descriptores de archivos de cada una de las tuberías, ya que enviar la dirección
        del primer elemento es como envíar el arreglo en sí mismo */
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
        /* Si pid del proceso hijo es cero, significa que fork creo bien el proceso hijo 
        ya qué fork() devuelve lo siguiente:
        1: Proceso padre.
        0: Proceso hijo.
        */
        if( !pid){
            /* Enviamos el número de proceso que se está ejecutando  y la dirección de 
               su propia tubería que se encuentra en arreglo de descriptores de archivo
               que creamos mediante la llamada al sistema pipe */
            proceso_hijo(np, &pipefd[np][0], datos);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre(pipefd);
    
    return 0;
}