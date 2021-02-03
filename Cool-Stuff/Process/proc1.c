#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>


int main(){
    float num1 = 45, num2 = 13, suma, resta;
    int status;
    printf("Probando procesos...\n");
    /* Identificador del proceso */
    pid_t pid;
    pid = fork();
    if( pid == -1){
        perror("Error al crear el proceso \n");
        exit(EXIT_FAILURE);
    } 

    /* ejecución del proceso hijo */
    if(!pid){
        printf("Proceso hijo con pid %d...\n", getpid());
        suma = num1 + num2;
        printf("La suma es: %f \n", suma);
        /* Con la función exit podemos terminar la ejecución de un proceso */
        exit(20);
        /*
         EL número de exit que pusimos aquí, la función wait lo recibe en su parámetro *wstatus
         Si ponemos un '1' se regresará el valor 256
         Si ponemos un '2' se regresará el valor de 512
        
             10 9 8 7 6 5 4 3 2 1 0 : Decimal
         0:  0  0 0 0 0 0 0 0 0 0 0 : 0
         1:  0  0 1 0 0 0 0 0 0 0 0 : 256
         2:  0  1 0 0 0 0 0 0 0 0 0 : 512
         Está ocurriendo un desplazamiento de 8bits a la izquierda (8<<)
         */

    }
    /* ejecución del proceso padre */
    else{
        printf("Proceso padre con pid: %d \n", getpid());
        resta = num1 - num2;
        printf("La resta es: %f \n", resta);
        /* Función: pid_t wait(int *wstatus); 
           Se encarga de hacer que el proceso que use esta función espere 
           a que termine la ejecución de todos los procesos que lanzó.
           - El argumento *status hace referencia al 'valor' que se colocó
           en la función exit('valor') para terminar la ejecución de un 
           proceso hijo, pero a este valor se le aplica un corrimiento de
           8 bits a la izquierda (<<8) por la función exit, por lo que, 
           si queremos el valor original que se ingreso en la función exit(), 
           sólo necesitamos realizar un corrimiento de 8 bits a la derecha (>>8)
           - Con esta función podemos sacar a un proceso del estado zombi.
           
           Retorno: Esta función retorna el identificador del proceso 'pid_t'
           que espero a que terminara su ejecución. Así mismo, si en el proceso
           hijo creado con 'fork' terminó su ejecución usando exit(valor), es
           posible recuperar ese valor del proceso hijo desde la función wait
        */
        pid = wait(&status);
        printf("PID del proceso hijo que termino: %d y estado: %d \n", pid, status>>8);

    }

    return 0;
}