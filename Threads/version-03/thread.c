#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_HILOS 4
/* Protocolos de las funciones */
void * funHilo( void *arg );
/* En esta versión de utilizan las variables globales */
int num1 = 20, num2 = 4;
int res_suma, res_resta, res_multi, res_divi;

int main()
{
	register int nh;
	pthread_t tids[NUM_HILOS];
	int *hilo, nhs[NUM_HILOS];

	printf("Ejemplo de hilos\n");

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
		/* pthread_join crea memoria para el apuntador que estamos pasando por referencia a la 
		función del hilo, nos regresa un apuntador con memoria creada, la variable retval tiene que ser
		un apuntador '*', pero pthread_join nos pide un doble apuntador el que hay que mandar, es decir,
		se tiene que mandar la dirección del apuntador (tenemos que poner el '&'), si se pone la dirección 
		del apuntador, entonces eso es un doble apuntador '**' */
		if (*hilo == 0)
			printf("La suma es: %d \n", res_suma);
		else if (*hilo == 1)
			printf("La resta es: %d \n", res_resta);
		else if (*hilo == 2)
		 	printf("La multiplicación es: %d \n", res_multi);
		else if (*hilo == 3)
			printf("La divisón es: %d \n", res_divi);
	}

	return 0;
}

void * funHilo( void *arg )
{
	int nh = *(int *)arg;

	printf("Hilo %d en ejecución \n", nh);
	/* EL indice me permita asignar tareas a cada uno de los hilos que se esta ejecutando */
	if( nh == 0 )
		res_suma = num1 + num2;
	else if( nh == 1 )
		res_resta = num1 - num2;
	else if( nh == 2 )
		res_multi = num1 * num2;
	else if( nh == 3 )
		res_divi = num1 / num2;

	/* Retornamos el número de hilo, que es el argumento que recibimos
	en esta función  */
	pthread_exit( arg );
}
