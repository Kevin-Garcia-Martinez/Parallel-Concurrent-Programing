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

float *A, *B, *C, *D;
int flag;

int main(){
    /* Register le indica al compilador que le asigne una variable un registro en lugar de una dirección de memoria */
    register int np;
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;
    int i;
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    B = reservarMemoria();
    C = reservarMemoria();
    D = reservarMemoria();

    /* Leemos los datos del Pulso Cardiaco */
    readDataFromFile( A, "pulso" );
    /* Guardamos la función Hann en un archivo */
    generateHannFunction( B );
    storeDataInFile( B, "Hann" );

    /* Inicializamos la bandera con cero */
    flag = 0;

    /* Proceso de ventaneo de la Señal del Pulso Cardiaco */
    for ( i = 0; i < 2; i++)
    {
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
        
        /* EL proceso padre recibe todo el arreglo de tuberías */
        proceso_padre(pipefd);
        /* Cambiamos el valor de la bandera */
        flag = 1;
    }
    
    /* Guardamos el resultado del proceso de ventaneo */
    storeDataInFile( C, "Ventaneo" );
    /* Guardamos el resultado de la Función de Autocorrelación */
    storeDataInFile( D, "Correlacion" );

    /* Liberamos la memoria de cada uno de los arreglos */
    free( A );
    free( B );
    free( C );
    free( D );
    
    return 0;
}


