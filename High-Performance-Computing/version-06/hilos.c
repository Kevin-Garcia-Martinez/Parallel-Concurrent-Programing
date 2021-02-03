/* Calcular promedio haciendo Paralelismo a nivel de datos */
/* PARALELISMO EN FORMA ALTERNADA */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
/* Constantes */
#define NUM_HILOS 4
#define N 16 /* Tamaño de bloque de 2 a la n se utiliza para utilizar recursos rápidos de operaciones
ya que al dividir por una potencia de 2 a la n, es lo mismo que hacer un corrimiento a la derecha */
/* Prototipos de las funciones */
void * funHilo( void * arg);
void imprimirArreglo( int *datos );
void llenarArreglo( int *datos );
int *reservarMemoria();


/* Cálculo del promedio de un bloque de datos */
int *A, promedio;
/* Declaramos el mutex */
pthread_mutex_t bloqueo;

int main(){
    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    /* Llenamos los arreglos */
    llenarArreglo( A );
    /* Imprimimos los arreglos */
    imprimirArreglo( A );
    /* incializamos el promedio para que no tenga basura */
    promedio = 0;
    /* incializamos el objeto de sincronización*/
    pthread_mutex_init(&bloqueo, NULL);

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
    /* Imprimiremos el promedio de los números que contiene el arreglo */
    /* Ya que 16 es igual a 2^4, entonces tenemos que realizar un corrimiento a la derecha de 4 bits
    que es lo mismo que dividir el promedio entre 16 elemento, ya que aprovechamos que 16 es una potencia
    de dos */
    promedio = promedio >>4;
    printf("El promedio es: %d \n", promedio);
    /* Liberamos la memoria reservada para los arreglos */
    free( A );
    /* Liberamos el mutex */
    pthread_mutex_destroy(&bloqueo);
    return 0;
}

/* Cada hilo realizará el producto de una sección del arreglo */
void * funHilo( void * arg){
    /* Número de hilo que entra a ejecutar esta función (nucleo)*/
    int nucleo = *(int *)arg;
    /* Variable del ciclo for */
    register int i;
    /* Esta suma parcial se creará por cada hilo que entre a la función del hilo*/
    int suma_parcial; /* Las variables locales no son compartidas por los hilos */
    /* Como promedio es una variable global, vamos a tener un problema de condición de carrera
    por lo que necesitamos utilizar un mutex para evitar que los hilos accedan al mismo tiempo
    a la variable global 'promedio' */
    printf("Hilo %d en ejecución \n", nucleo);
    suma_parcial = 0;
    /* Se realiza la paralelización a nivel de datos, ahora el ciclo for se ejecuta de manera paralela */
    /* Establecemos nuestra sección crítica, pero lo que va a suceder es que el problema
    se hará de la manera secuencial, por lo que para evitar esto, debemos tener encuenta que 
    las sumas se pueden paralelizar */
   
    for( i=nucleo; i<N; i+= NUM_HILOS )
    {
        /* Cada hilo procesa en paralelo la suma parcial del bloque que le toco */
        suma_parcial += A[i];
    }
     pthread_mutex_lock(&bloqueo);
     /* Suma parcial hay que añadirlo al promedio, por lo que el promedio será la única variable
     que tengo que proteger con mutex, de este modo el proceso de suma se realiza de manera paralela
     y ya cada vez que termine un hilo de realizar su suma por bloque deberá almacenar el resultado en 'promedio'
     pero uno a la vez*/
     promedio += suma_parcial; /* Se ocupa un mutex para que los hilos escriban correctamente su resultado */
     pthread_mutex_unlock(&bloqueo);
    
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
        printf("%3d ", datos[i]);
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
