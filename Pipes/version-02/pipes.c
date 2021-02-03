#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#define NUM_PROC 4

/* Cada proceso hijo recibirá la tubería que le corresponde */
void proceso_hijo( int np, int pipefd[]);
/* El proceso padre recibe un arreglo de tuberías  */
void proceso_padre( int pipefd[NUM_PROC][2] );

/*
 En el programa anterior 'pipe.c' se definió una tubería para 4 procesos, pero surgió el problema 
 de que en ocasiones los procesos no se ejecutaban de la manera que esperábamos. Y esto 
 se debe a que el planificador de procesos del sistema operativo decide el orden en que
 se lanzarán los procesos que creemos con la llamada el sistema fork(), es por esta razón
 que cuando obtenemos los valores de la tubería no se encuentran en el orden en que se 
 esperaba que se escribieran dentro de esta. Para evitar este problema se crearán 4 tuberías
 , es decir, una tubería para cada uno de los procesos que se creo. Para garantizar que
 siempre vamos a leer el valor correcto que esperamos obtener de un proceso hijo.
*/

int main(){
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
            proceso_hijo(np, &pipefd[np][0]);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre(pipefd);

    return 0;
}


/* EL proceso hijo recibe una tubería que será exclusiva para este */
void proceso_hijo( int np, int pipefd[]){
    // Los descriptores se cierran sólo en el proceso hijo, el proceso padre puede seguir utilizándolos con normalidad
    int num1 = 20, num2 = 4;
    int suma, resta, multi, divi;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    /* Con el número de proceso 'np', podemos asignar tareas a los procesos hijos que creamos en el proceso padre*/
    if(np==0){
        suma = num1 + num2;
        /* Escribimos el resultado en la tubería, enviamos un apuntador ya que queremos que el valor que se envie
        el valor que se encuentra dentro de esa dirección de memoria */
        write(pipefd[1], &suma, sizeof(int));
    }
    /* Si se trata del proceso 1 realiza la suma */
    else if(np==1){
        resta = num1 - num2;
        write(pipefd[1], &resta, sizeof(int));
    }

    else if(np==2){
        multi = num1 * num2;
        write(pipefd[1], &multi, sizeof(int));
   }
    else if(np==3){
        divi = num1 / num2;
        write(pipefd[1], &divi, sizeof(int));
    }


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
    int resultado;
    register int np;
    
    /* Pasamos a wait una dirección de memoria para que en esta devuelva
    el valor que regreso el proceso hijo en la función exit(), en este caso
    es el número de proceso. */
    for( np=0; np<NUM_PROC; np++){
        /* EL número de proceso 'numproc' que devuelve wait es el que utilizamos para asignarles 
        tareas a cada uno de los procesos hijo */
        pid = wait(&estado);
        /* Hacemos el corrimiento para obtener el número de proceso que envío el proceso hijo */
        numproc = estado>>8; 
        /* Cerramos el descriptor de archivo de escritura, ya que no lo utilizamos en el proceso padre */
        close( pipefd[numproc][1] );
        
        /* Leemos el valor que puso el proceso hijo en la tubería y lo guardamos en la variable resultado,
        es por eso que lo enviamos como un apuntador. */
        read( pipefd[numproc][0],&resultado, sizeof(int) );
        /* Si el proceso que acaba de terminar es el proceso hijo 0 */
        if( numproc == 0){
            printf("Termino el proceso %d con pid: %d \n", numproc, pid);
            printf("La suma es: %d \n", resultado);
        }

        else if( numproc == 1){
            printf("Termino el proceso %d con pid: %d \n", numproc, pid);
            printf("La resta es: %d \n", resultado);
        }

        else if( numproc == 2){
            printf("Termino el proceso %d con pid: %d \n", numproc, pid);
            printf("La multiplicación es: %d \n", resultado);
        }

        else if( numproc == 3){
            printf("Termino el proceso %d con pid: %d \n", numproc, pid);
            printf("La división es: %d \n", resultado);
        }

         /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
        close( pipefd[numproc][0] );


        
    }

   

}


