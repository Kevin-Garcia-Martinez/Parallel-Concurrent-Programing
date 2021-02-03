#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#define NUM_PROC 4

void proceso_hijo( int np, int pipefd[]);
void proceso_padre( int pipefd[] );
/* En lectura no ocurre el problema de que los procesos escriban en desorden 
en la tubería, ya que solamente estamos leyendo. */
int main(){
    pid_t pid;
    int pipefd[2]; /* Descriptores pipefd[0] para leer y escribir pipefd[1] de la tubería */ 
    int edo_pipe;
    register int np;

    /* Creamos una tubería, la función pipe devuelve 1 si se creo exitosamente*/
    edo_pipe = pipe( pipefd);
    printf("Probando procesos.... \n");
    
    if( edo_pipe == -1){
        perror("Error al crear la tubería \n");
        exit(EXIT_FAILURE);
    }

    /* Generamos procesos hijos */
    for (np = 0; np < NUM_PROC; np++)
    {
        pid = fork();
        if( pid == -1){
            perror("Error al crear el proceso \n");
            exit(EXIT_FAILURE);
        } 
        /* Código del proceso hijo */
        if( !pid){
            /* Le pasamos el número de proceso que se esta ejecutando */
            proceso_hijo(np, pipefd);
        }
        
    }

    /* El proceso padre continuará ejecutando lo que hay después del ciclo for*/
    proceso_padre(pipefd);

    return 0;
}


/* Terminamos la ejecución del proceso hijo */
void proceso_hijo( int np, int pipefd[]){
    /* Los descriptores se cierran sólo en el proceso hijo, el proceso padre puede seguir 
    utilizándolos con normalidad */
    int num1 = 20, num2 = 4;
    int suma, resta, multi, divi;

    /* Cerramos el descriptor de lectura porque no lo vamos a utilizar en el proceso hijo */
    close( pipefd[0]);
    /* Con np podemos asignar tareas a los procesos hijos que creamos en el proceso padre*/
    if(np==0){
        suma = num1 + num2;
        /* Escribimos el resultado en la tubería */
        write(pipefd[1], &suma, sizeof(int));
    }
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
     /* Devolvemos el número de proceso hijo que está terminando su ejecución */
    exit(np);
}

/* Liberaremos a los procesos hijos que se encuentran en estado Zombie */
void proceso_padre( int pipefd[] ){
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado, numproc, resultado;
    register int np;
    /* Cerramos el descriptor de archivo de escritura, ya que no lo utilizamos en el padre */
    close( pipefd[1] );
    /* Pasamos a wait una dirección de memoria para que en esta se escriba el valor que regreso 
    el proceso hijo en la función exit() */
    for( np=0; np<NUM_PROC; np++){
        pid = wait(&estado);
        /* Hacemos el corrimiento para obtener el número explícito que se envio desde el hijo con exit */
        numproc = estado>>8;
        /* Leemos el valor que puso el proceso hijo en la tubería */
        read( pipefd[0],&resultado, sizeof(int) );
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
        
    }

    /* Cerramos el descriptor de lectura porque ya lo terminamos de utilizar*/
    close( pipefd[0] );
    /* Los 4 procesos hijos se ejecutan al mismo tiempo, depende de como los lanze el 
    procesador, puede que lanze unos primero y después otros, pero el planificador del 
    sistema operativo se encarga de ejecutar cada proceso, no necesariamente se ejecutarán 
    en orden:
    0,1,2,3 puede ser 1,3,2,0 y así....*/

}

