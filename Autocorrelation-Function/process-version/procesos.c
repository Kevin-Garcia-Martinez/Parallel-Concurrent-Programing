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
extern float *A, *B, *C, *D;
extern int flag;


void proceso_padre( int pipefd[NUM_PROC][2] ){
    register int np;
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado; 
    /* A través de este identificador sabremos cuál es el número de tubería que debemos leer de acuerdo 
    al número de proceso recuperado con wait */
    int numproc;

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
        
        if ( flag == 0 )
            read( pipefd[numproc][0], C + inicio , sizeof(float)*eleBloque );
        else
            read( pipefd[numproc][0], D + inicio , sizeof(float)*eleBloque );
        
        printf("Termino el proceso %d con pid: %d \n", numproc, pid);  
         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );
    }
    printf("\n");
}

/* En los procesos no existen condiciones de carrera, ya que cada proceso
 * trabaja con una copia de los datos de manera independiente. 'np' será 
 * nuestro nuclero ahora y con el se determinará el inicio y el fin de 
 * cada bloque que procesará cada proceso. 
 */
void proceso_hijo( int np, int pipefd[]){
    /* Calculamos el número de elementos por bloque */
    int eleBloque = NUM_DATOS/NUM_PROC;
    /* Calculamos el inicio de bloque de cada proceso */
    int iniBloque = np * eleBloque;
    /* Calculamos el fin de bloque de cada proceso */
    int finBloque = iniBloque + eleBloque;
    register int i, l, n;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
 
    if ( flag == 0){
        for ( i = iniBloque; i < finBloque; i++){
            C[i] = A[i] * B[i];
        }
        write( pipefd[1], C + iniBloque, sizeof(float)*eleBloque);
    }

    else{
        for (l = iniBloque; l < finBloque; l++)
        {
            for (n = l; n <= NUM_DATOS-1; n++)
            {
                D[l] += C[n] * C[n-l];
            }
        }

         write( pipefd[1], D + iniBloque, sizeof(float)*eleBloque);
    }
    
    /* Cerramos el descriptor de escritura porque ya no lo vamos a utilizar */
    close( pipefd[1]);

    /* Retornamos el número de proceso hijo que termino su ejecución, estos procesos hijo ya
     no ejutarán más código apartir de aquí */
    exit(np);
}




