/* PARALELISMO EN BLOQUES */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
/* Constantes */
#define NUM_HILOS 4
#define N 16
/* Prototipos de las funciones */
void * funHilo( void * arg);
void imprimirArreglo( int *datos );
void llenarArreglo( int *datos );
int *reservarMemoria();
int *A, *B, *C;

int main(){
    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    B = reservarMemoria();
    C = reservarMemoria();
    /* Generamos la semilla, esta semilla la toma como valor la función rand */
    srand(getpid()); 
    /* Llenamos los arreglos */
    llenarArreglo( A );
    llenarArreglo( B );
    /* Imprimimos los arreglos */
    imprimirArreglo( A );
    imprimirArreglo( B );

    /* En los hilos la memoria es compartida */
    for (nh = 0; nh < NUM_HILOS; nh++){  
        nhs[nh] = nh;
        pthread_create( &tids[nh], NULL, funHilo, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&hilo);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El hilo termino su ejecución %d \n", *hilo);
    }

    imprimirArreglo( C );
    /* Liberamos la memoria reservada para los arreglos */
    free( A );
    free( B );
    free( C );
    
    return 0;
}

/* Cada hilo realizará el producto de una sección del arreglo */
void * funHilo( void * arg){
    /* Número de hilo que entra a ejecutar esta función (nucleo)*/
    int nucleo = *(int *)arg;
    /* Número de elementos por bloque, es decir, el tamaño de bloque */ 
    int elemBloque = N / NUM_HILOS; /* Número de datos dividido por el número de hilos que tenemos */
    /* Calculamos el inicio de bloque, en donde comenzará cada hilo a realizar la multiplicacion del subarreglo */
    int iniBloque = nucleo * elemBloque;
    /* Calculamos el fin de bloque */
    int finBloque = iniBloque + elemBloque;
    /* Variable del ciclo for */
    register int i;

    printf("Hilo %d en ejecución \n", nucleo);
    /* Se realiza la paralelización a nivel de datos, ahora el ciclo for se ejecuta de manera paralela */
    for( i=iniBloque; i<finBloque; i++ )
    {
        /* No hay problemas de condición de carrera, debido
        a que los indices son distintos para cada hilo que entrará
        a ejecutar la función */
        C[i] = A[i] * B[i];
    }

    /* Retornaremos el resultado de la operación que realizó cada uno de los hilos */
    pthread_exit( arg );
}

void imprimirArreglo( int *datos ){
    register int i;
    for ( i = 0; i < N; i++){
        if(!(i%16)){
            printf("\n");
        }
        /* %3d imprime los números como si fueran de
           3 caracteres de longitud.   */
        printf("%5d ", datos[i]);
    }
    printf("\n");
    
}

void llenarArreglo( int *datos ){
    register int i; 
    /* Llenamos nuestro arreglo */
    for(i=0; i<N;i++){
        datos[i] = rand()%256;
    }
}

int *reservarMemoria(){
    int * mem;
    /* malloc reserva una cantidad de bytes y devuelve un apuntador de tipo void*/
    mem = (int *) malloc( N*sizeof(int) );
    /* Retornamos la memoria si es que se asignó correctamente*/
    if(!mem){ /* si la memoria no se asigno 'null'*/
        perror("Error al asignar memoria \n");
        exit(EXIT_FAILURE);
    }

    return mem;
}
