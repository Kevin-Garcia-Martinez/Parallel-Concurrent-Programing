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
/* Declaramos promedio */
int promedio;

int *A;

int main(){
    pid_t pid;
    /* Creamos 4 tuberías y cada tendrá un descriptor de archivo para lectura pipefd[0] y escritura pipefd[1] */
    int pipefd[NUM_PROC][2];
    int edo_pipe;
    register int np;
    /* Reservamos memoria para los arreglos */
    A = reservarMemoria();
    /* Llenamos los arreglos */
    llenarArreglo( A );
    /* Imprimimos los arreglos */
    imprimirArreglo( A );
    /* Inicializamos el promedio */
    promedio = 0;
    
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
     
    /* Imprimimos el promedio */
    printf("El promedio es: %d\n", promedio);
    
    /* Liberamos la memoria de cada uno de los arreglos */
    free( A );

    return 0;
}

/* Procesamiento/Paralelismo a nivel de datos usando bloques 
Se divide el conjunto de datos (arreglo) en bloques. Por bloques
la memoria es continua, entonces eso facilita que podamos regresar
esos datos (memoria continua) por una tubería.

Para el cálculo del promedio el resultado del algoritmo siempre será un
sólo número, así que no importa si los datos están consecutivos o salteados 
en memoria, ya que la suma siempre será la misma. 

*/
void proceso_hijo( int np, int pipefd[]){
    /* np será nuestro núcleo ahora y con el determinaremos el inicio y fin de cada bloque
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
    /* Inicializamos el promedio para evitar que este tenga basura  */
    promedio = 0;
    for ( i = iniBloque; i < finBloque; i++)
        promedio+= A[i]; /* Esta variable promedio es diferente para cada proceso hijo (incluso el padre)*/

    /* Le retornamos la dirección de memoria del 'promedio' que calculo este proceso hijo
    al proceso padre, para que este pueda calcular el promedio final del bloque de datos */
    write( pipefd[1], &promedio, sizeof(int));
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
        promedio+=suma_parcial;
        /* Si el proceso que acaba de terminar es el proceso hijo 0 */
        printf("Termino el proceso %d con pid: %d \n", numproc, pid);  
         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );
    }
   /* Un corrimiento a la derecha es lo mismo que realizar una división */
    promedio = promedio >> 4; 

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

