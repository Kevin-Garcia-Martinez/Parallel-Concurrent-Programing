#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/* En los hilos la memoria es compartida */

/* Protocolos de las funciones */
void *suma( void * arg);
void *resta( void * arg);
void *multi( void * arg);
void *divi( void * arg);

/* Variables globales */
int num1 = 20, num2 = 4;


int main(){
    int *res_suma, *res_resta, *res_multi, *res_divi;
    /* pthread_t es el identificador del hilo */
    pthread_t tid_suma, tid_resta, tid_multi, tid_divi;
    /* Creamos un nuevo hilo con 'pthread_crete', el nombre de la función es 
    la dirección de memoria de esta, ya que esta apunta a la primera instrucción 
    dentro de la función */
    pthread_create( &tid_suma, NULL, suma, NULL);
    pthread_create( &tid_resta, NULL, resta, NULL);
    pthread_create( &tid_multi, NULL, multi, NULL);
    pthread_create( &tid_divi, NULL, divi, NULL);
    
    printf("Probando hilos..");
    /* La función 'phtread_join' nos permite esperar la terminación de un hilo desde un 
    proceso, es decir, el proceso principal detiene su ejecución hasta que todos los hilos
    creados hayan terminado su ejecución. Recibe los siguiente parámetros:

    Primero: Identificador del hilo que queremos esperar a que termine su ejecución
    Segundo: Es el valor de retorno que nos devolverá el hilo, este se rcibe como un 
    doble apuntador */
    pthread_join(tid_suma, (void **)&res_suma);
    pthread_join(tid_resta, (void **)&res_resta);
    pthread_join(tid_multi, (void **) &res_multi);
    pthread_join(tid_divi,  (void **) &res_divi);
    /* Para tomar el contenido de un apuntador utilizamos nuevamente el operador '*' */
    printf("La suma es: %d \n", *res_suma);
    printf("La resta es: %d \n", *res_resta);
    printf("La multiplicación es: %d \n", *res_multi);
    printf("La divisón es: %d \n", *res_divi);
    
    /* Se libera la memoria que se reservo para los resultados del hilo. */
    free( res_multi );
    free( res_divi );

    return 0;
}
/* -lpthread parte del enlazador */

/* FUnción que regresa un apuntador void y recibe un apuntador void */
void *suma( void * arg){
    /* Si dejamos la variable con 'int', el contenido de la variable se desruye, ya que 
    una vez  que la función termina, las variables locales de la función desaparecen 
    
    static' perrmite que la variable local conserve su valor a pesar de que haya terminado 
    la ejecución de la función */
    static int res;
    res = num1 + num2;
    /* La función pthread_exit se pone al final del código que ejecutará el hilo para terminar
    la ejecución del hilo. Lo que devuelve esta función que ejecuta el hilo, lo podemos recuperar 
    en el proceso princiál mediante la función pthread_join */
    pthread_exit( (void *) &res); /* Equivalente al exit que ocupamos en procesos*/ 
}

void *resta( void * arg){
    static int res;
    res = num1 - num2;
    /* La función pthread_exit se pone al final del código que ejecutará el hilo 
    Lo que devuelve esta función es lo que se obtendrá mediante pthread_join */
    pthread_exit( (void *) &res);
}

/* Si queremos retornar una variable local de una función mediante un hilo sin perder
su contenido, lo podemos hacer ya sea utilizando 'static' o reservando memoria dinámica */
void *multi( void * arg){
    /* Podemos reservar memoria a la variable para que no se pierda su contenido una 
    vez termine la función */
    int *res = (int *) malloc(sizeof(int));
    /* guardamos la suma en el contenido de la variable res que ahora es un apuntador */
    *res = num1 * num2;
    /* La función pthread_exit se pone al final del código que ejecutará el hilo 
    Lo que devuelve esta función es lo que se obtendrá mediante pthread_join */
    pthread_exit( (void *) res);
}

void *divi( void * arg){
    /* Al reservar memoria a esta variable, ahora esta existirá fuera de la función
    pero, tenemos que liberar la memoria una vez que dejemos de utilizarla */
    int *res = (int *) malloc(sizeof(int));
    *res = num1 / num2;
    /* La función pthread_exit se pone al final del código que ejecutará el hilo 
    Lo que devuelve esta función es lo que se obtendrá mediante pthread_join */
    pthread_exit( (void *) res);
}





