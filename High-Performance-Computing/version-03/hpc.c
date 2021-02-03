#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#define NUM_PROC 4
#define N 16
/* Cada proceso hijo recibirá la tubería que le corresponde */
void proceso_hijo( int np, int pipefd[]);
/* El proceso padre recibe un arreglo de tuberías  */
void proceso_padre( int pipefd[NUM_PROC][2] );
/* Prototipos de las funciones */
void imprimirArreglo( int *datos );
void llenarArreglo( int *datos );
int *reservarMemoria();
int *A, *B, *C;

int main(){
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;
    register int np;
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    B = reservarMemoria();
    C = reservarMemoria();
    /* Llenamos los arreglos */
    llenarArreglo( A );
    llenarArreglo( B );
    /* Imprimimos los arreglos */
    imprimirArreglo( A );
    imprimirArreglo( B );
    
    /* Generamos los procesos hijos */
    for (np = 0; np < NUM_PROC; np++)
    {
        /* Se crea un tubería por cada proceso hijo */
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
        
        if( !pid){
            /* Enviamos el número de proceso que se está ejecutando  y la dirección de 
               su propia tubería */
            proceso_hijo(np, &pipefd[np][0]);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre(pipefd);
    /* EL proceso padre recibe todo el arreglo de tuberías */
    imprimirArreglo( C );
    
    /* Liberamos la memoria de cada uno de los arreglos */
    free( A );
    free( B );
    free( C );

    return 0;
}


/* EL proceso hijo recibe una tubería que será exclusiva para este */
 /*
 Lo que los procesos hijos hagan con sus datos es completamente 
 independiente del proceso padre, es por eso que utilizamos un 
 mecanismo de IPC para retornar los datos que proceso el proceso hijo
 al proceso padre y este se encargue de recolectarlos y ordenarlos. 
 
 En los procesos no existen condiciones de carrera, ya que cada proceso
 trabaja con una copia de los datos de manera independiente
 */


void proceso_hijo( int np, int pipefd[]){
    /* np será nuestro nuclero ahora y con el determinaremos el inicio y fin de cada bloque
    que tendrá cada proceso, se le llama nucleo porque vamos a trabajar con los núcleos físicos
    del procesador. */
    /* Calculamos el número de elementos */
    int eleBloque = N/NUM_PROC;
    /* Calculamos el inicio de bloque de cada procesos */
    int iniBloque = np*eleBloque;
    /* Calculamos el fin de bloque de cada procesos */
    int finBloque = iniBloque + eleBloque;
    register int i;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    
    /* Con procesos es conveniente trabajar con paralelismo en bloques, ya que los bloques son 
    continuos en memoria y podemos regresar esos 'bloques' a través del pipe.*/ 
    
    for ( i = iniBloque; i < finBloque; i++)
    {
        /* Cada proceso trabajará con un subarreglo, con un fragmento del arreglo, no hay que 
        olvidar que en un proceso las variables globales no son compartidas, es decir, se hace 
        una copia diferente de cada una de estas variables para cada proceso, es decir, son 
        distintas a la original */
        C[i] = A[i] * B[i];
    }

    /* inicioBloque tiene el inicio del bloque que ejecuto cada uno de los procesos. 
    Como 'C' es la dirección de memoria del arreglo al sumarle el inicio  de Bloque 'iniBloque' 
    nos estamos desplazando por este arreglo hasta llegar al inicio de Bloque que proceso cada 
    uno de los procesos, para que así podamos escribir la dirección de memoria correspondiente 
    al inicio de bloque de cada de proceso en la tubería*/
    write( pipefd[1], C + iniBloque, sizeof(int)*eleBloque);
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
    /* Número de elementos por bloque */
    int eleBloque = N / NUM_PROC, inicio;

    
    for( np=0; np<NUM_PROC; np++){
        /* EL número de proceso 'numproc' que devuelve wait es el que utilizamos para asignarles 
        tareas a cada uno de los procesos hijo */
        pid = wait(&estado); /* El número de proceso que devuelve el proceso hijo mediante exit() se almacena en 'estado' */
        /* Hacemos el corrimiento para obtener el número de proceso que envío el proceso hijo */
        numproc = estado>>8; 
        /* Cerramos el descriptor de archivo de escritura, ya que no lo utilizamos en el proceso padre */
        close( pipefd[numproc][1] );
        /* Se calcula el inicio de acuerdo al valor que devuelte el proceso hijo con exit() */
        inicio = numproc*eleBloque;
        /* Leemos el bloque que proceso cada proceso hijo apartir del inicio de bloque */
        read( pipefd[numproc][0], C+inicio , sizeof(int)*eleBloque );
        /* Si el proceso que acaba de terminar es el proceso hijo 0 */
        printf("Termino el proceso %d con pid: %d \n", numproc, pid);  
         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );
  
    }

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

