#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_HILOS 4
/* Protocolos de las funciones */
void * funHilo( void *arg );
/* Zona de variables globales */
int contador;
/* Colocamos el objeto de sincronización */

pthread_mutex_t bloqueo;

int main()
{
	register int nh = 0;
	pthread_t tids[NUM_HILOS];
	int *hilo, nhs[NUM_HILOS];
	/* Inicializamos el contador en 0 */
	contador = 0;
    /* Inicializamos el mutex de manera dinámica, como primer parámetro recibe 
	atributos extra para su inicialización, como segundo parámetro recibe la dirección del
	objeto de sincronización que creamos */
	pthread_mutex_init(&bloqueo, NULL);


	for( nh = 0; nh < NUM_HILOS; nh++ )
	{
		nhs[nh] = nh;
		/* Si queremos enviar más argumentos a la función del hilo lo que se hace es construir una estructura y 
		simplemente mandar la dirección de la estructura de datos */
		pthread_create( &tids[nh] , NULL, funHilo, (void *)&nhs[nh] );
	}

	for( nh = 0; nh < NUM_HILOS; nh++ )
	{
		/* Identificamos el número de hilo que termino de ejecutarse */
		pthread_join( tids[nh], (void **)&hilo );
		/* Solo imprimimos que hilo termino su ejecución */
		printf("Hilo %d terminado \n", *hilo);
		/* La función printf utiliza la salida de vídeo para imprimir la salida 
		de nuestro hilo, como hay varios printf puede ocurrir que algunos printf
		se manden a dormir porque quizás otros proceso en ese momento esta utilizando 
		salida de vídeo  */
		
	}
	 /* Debemos destruir el objeto de sincronización */
	pthread_mutex_destroy( &bloqueo );

	return 0;
}

void * funHilo( void *arg )
{
	/* 
	Reentrancia de una función: Dos hilos quieren usar la misma función 
	Hilo 1			Hilo 2				Hilo 3			  
	sleep(1)       sleep(1)             sleep(1)    
	cont_time++    cont_time++          cont_time++
	0...34872      0.... 21732          0.... 14762     

    Como podemos ver el valor del cont_time se ve modificada por cada uno de los hilos
	Cuando una función puede recuperar de una instrucción de descanso, es decir
	continua exactamente en donde se quedo. 
    No es reentrante: sleep(1);
	*/
	register int i=0; /* 32 bits */
	int nh = *(int *)arg;
    /* Modificamos el valor del contador, en un caso ideal deberíamos
	de ver un 1 */
	/* Aqui comienza la sección critica, la sección que modificaran los hilos, bloqueara a los demás hilos para que no puedan acceder a esta zona*/
	pthread_mutex_lock(&bloqueo);
	contador++;
	printf("Hilo %d en ejecución con contador %d \n", nh, contador);

	/* 
	Condificón de carrera: Un hilo utiliza la variable que otro hilo
	esta utilizando, es decir, el hilo 1 utiliza la varibale 'contador'
	cuando el hilo 0 aún está ocupando esa variable.   
	
	i comienza en cero, lo primero que se hace es el decremento, si a cero 
	le resto un 0 - 1 = -1
	-1 en binario es: 

	0 0 0 1 = 1 en binario

	Complemento a 2:
	1 1 1 0 complemento a uno (negar los bits)
    0 0 0 1 sumamos el '1' 
	-------
	1 1 1 1 = -1 este es el valor de '-1' en complemento a dos
   
    En este ciclo while tendremos 32 unos encendidos 
	1111 1111 1111 1111 1111 1111 1111 1111 = este es el valor de '-1'
	Entonces 'i' se convierte en el número máximo que puede tomar el registro
	de 32 bits = 2^32 iteraciones ejecutará este ciclo. 
	En el lapso que el hilo esta ejecutando el ciclo while, el planificador lo manda
	a dormir hasta que termine la ejecución del ciclo, es decir, entra en nuestro retardo
 
	 */
	while((--i));

	contador++;
	printf("Hilo %d en ejecución con contador %d \n", nh, contador);
    /* Liberamos la sección crítica para que otro hilo pueda acceder a ella */
	pthread_mutex_unlock(&bloqueo);
	
   

	/* Retornamos el número de hilo, que es el argumento que recibimos
	en esta función  */
	pthread_exit( arg );
}

/*
 Práctica: 

Realizar un programa que genere 4 hilos y un conjunto de 128 valores enteros. 
Considere lo siguiente:

a) Usar un arreglo GLOBAL para los 128 valores enteros (*datos), con la intención que todos los
hilos puedan acceder a ese arreglo global. 

Cada hilo va a ejecutar un algoritmo de los siguientes:
- Ordenamiento.
- Promedio. 
- Números Pares.
- Calcular la cantidad de números primos del arreglo

b) El proceso padre (main) debe imprimir los resultados de cada hilo. 
Los resultados deben colocarse en variables globales. (para que hagamos uso del comando extern)

Se declara en el principal.c el arreglo y en el archivo procesamiento.c en donde estarán las funciones del hilo 
Al colocar el programa en módulos, tenemos que compartir las variables globales de un archivo en otro, y eso 
lo hacemos haciendo uso de extern. 

Principal.c          Procesamiento.c
int *datos           extern int *datos (Con extern le decimos al compilador que este arreglo ya fue declarado en otro archivo diferente, es decir, el arreglo no se vuleve a colocar en memoria, solocamente queremos usarlo en este archivo )
int main(){          La función ya no recibirá el arreglo, porque ahora es global, y ahora sólo lo unico que le pasaremos es un número de hilo 
	                 int promedio() {
                        for(){
                          suma += datos[i]
						}
}                    }

La variable declarada con extern de manera global ya tendrá todos los 
datos que se modificaron o asignaron en donde se declaro
la variable sin extern

 */

