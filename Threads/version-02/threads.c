#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
/* Constantes */
#define NUM_HILOS 4
/* Protocolos de las funciones */
void * funHilo( void * arg);

/*              Zona de variables globales 

El proceso main se encargará de convertir esta zona en una zona de memoria 
compartida para todos los hilos que se creen en el proceso principal.  */

/* Compartir una variable entre los hilos que se están ejecutando  */

int main(){
    register int nh; 
    /* La función 'pthread_create' puede enviar argumentos a la función que ejecutará el 
    hilo, esto lo hace con su parámetro 'void *arg', pero lo que está enviando es una 
    dirección de memoria, por lo que, nuestro objetivo es enviar una dirección de memoria
    diferente para cada uno de los hilos que se crearán, con el fin de que estos no se
    queden con la misma dirección de memoria siempre. */
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crearan */
    pthread_t tids[NUM_HILOS];
    /* En esta variable almacenaremos el valor deuvelto por join */
    int *resultado;

    /* En los hilos la memoria es compartida */
    for (nh = 0; nh < NUM_HILOS; nh++){   
        /* En la primera iteración dentro de la variable nhs[0] se almacenará el cero 
         *   nhs[0] = 0 
         * Y así con las demás...
         *  nhs[1] = 1 
         *  nhs[2] = 2 
         *  nhs[3] = 3 
         * En cada iteración se está asignando un valor a cada elemento del arreglo, es decir, 
         * se están inicializando 4 direcciones de memoria distintas en donde cada una tendrá
         * almacenada el número del hilo que se está ejecutando. 
         */
        nhs[nh] = nh;
        
        /* Mandamos la dirección de memoria de cada uno de los identificadores de los hilos, podemos 
        enviar un parámetro (void *arg) extra a la función que va a ejecutar el hilo, y se envía la 
        dirección de memoria de ese parámetro extra, es decir, paso por referencia, por esta razón
        la dirección de memoria del parámetro extra que enviaremos a la función del hilo debe ser 
        diferente para cada hilo, ya que si siempre dejamos la misma, los hilos comparten direcciones 
        de memoria, por lo tanto, siempre se enviaría la misma dirección y su valor no cambiaría. */
        pthread_create( &tids[nh], NULL, funHilo, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&resultado);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El resultado es: %d \n", *resultado);
        /* Libreamos la memoria que reservamos en la función del hilo */
        free( resultado );
    }
    
    return 0;
}


/* Esta función recibirá como argumento el número de hilo que se está creando 
desde el proceso principal 'main', con el objetivo de poder asignar una tarea 
específica a cada uno de los hilos dentro de esta función. */

void * funHilo( void * arg){
    /* El valor que pasamos a la función del hilo se recibe en el argumento con 'void *arg' 
    por lo tanto, necesitamos realizar un casteo y como sólo queremos el valor de esa dirección
    de memoria utilizamos el operador '*'*/
    int nh = *(int *)arg; 
    int num1 = 20, num2 = 4;
    /* Al reservar memoria dinámicamente para la variable 'res', garantizamos que cuando un hilo lea
    esa línea de código, esta reserve una dirección de memoria diferente por cada hilo que la ejecute, 
    de este modo hacemos que cada uno de los hilos guarde sus resultados en zonas de memoria diferentes */
    int *res = (int *) malloc(sizeof(int));    
    printf("Hilo %d en ejecución \n", nh);
    /* Aquí se hace la paralelización de los hilos */
    if( nh == 0)
    /* Como la variable 'res' es un apuntador, para almacenar un dato en esta lo tenemos que hacer con
    '*res', de este modo estaremos almacenando el resultado dentro de esa dirección de memoria */
        *res = num1 + num2;
    else if ( nh == 1)
        *res = num1 - num2;  /* En esta sección estamos realizando la paralelización de procesos */
    else if (nh == 2)
        *res = num1 * num2;
    else if (nh == 3)
        *res = num1 / num2;

    /* Retornaremos el resultado de la operación que realizó cada uno de los hilos */
    pthread_exit( (void *) res );
}
