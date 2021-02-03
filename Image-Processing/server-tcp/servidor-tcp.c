#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
/*Se pone entre comillas porque hace referencia a la carpeta
en la que estamos compilando, es decir, toma la carpeta donde 
estamos para abrir este archivo de cabecera. */
#include "imagen.h"

#define PUERTO 			5000	// Número de puerto asignado al servidor
#define COLA_CLIENTES 		5 	// Tamaño de la cola de espera para clientes
#define TAM_BUFFER 		100
#define EVER 			1

void manejador( int sig );
int finPrograma;
void atiendeCliente( int cliente_sockfd, unsigned char *imagenGray, bmpInfoHeader *info);
/* La función iniServidor retornara el descriptor del socket */
int iniServidor( );
void iniSignals( );

void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height );
unsigned char * reservarMemoria( uint32_t width, uint32_t height );

int main(int argc, char **argv)
{

	pid_t pid;
   	int cliente_sockfd, sockfd;
	bmpInfoHeader info;
	unsigned char *imagenRGB, *imagenGray, *imagenFiltrada;

	imagenRGB = abrirBMP("huella1.bmp", &info );

	displayInfo( &info );

	imagenGray     = reservarMemoria( info.width, info.height );
	imagenFiltrada = reservarMemoria( info.width, info.height );
	memset( imagenFiltrada, 255, info.width*info.height );

	RGBToGray( imagenRGB, imagenGray, info.width, info.height );


	/* Bandera declarada de manera global */
    finPrograma = 0;
	iniSignals( );

	sockfd = iniServidor( );
/*
 *  accept - Aceptación de una conexión (Función bloqueante )
 *  Selecciona un cliente de la cola de conexiones establecidas
 *  se crea un nuevo descriptor de socket para el manejo
 *  de la nueva conexion. Apartir de este punto se debe
 *  utilizar el nuevo descriptor de socket
 *  accept - La función es BLOQUEANTE
 */
 
 /* Mientras no sea el fin de programa, se quedará recibiendo clientes */
	for(;!finPrograma;)
	{
	   	printf ("En espera de peticiones de conexión ...\n");
   		/* Accept. Función del servidor bloqueante, es decir, bloquea el socket hasta que reciba un cliente */
		   if( (cliente_sockfd = accept(sockfd, NULL, NULL)) < 0 )
		{
			perror("Ocurrio algun problema al atender a un cliente");
			exit(1);
   		}
		/* Se crea un proceso hijo para cada cliente que se conecte, por lo tanto, acepará múltiples clientes */
		pid = fork();
		/* Se ejecuta el proceso del cliente */
		if( !pid )
		{
			/* Enviaremos la imagen al cliente */
			atiendeCliente( cliente_sockfd, imagenGray, &info  );
			/* Después de que se atienda el cliente, se va a liberar la memoria */
            
		}
	}
    
	/* Se cierra el socket */
	close (sockfd);
	/* Se libera la memoria utilizada por el cliente (con la que le mandamos la imagen)*/
	free( imagenRGB );
	free( imagenGray );
	

	return 0;
}

void iniSignals( )
{
	if( signal( SIGCHLD, manejador) == SIG_ERR )
	{
		perror("Error en el manejador");
		exit(EXIT_FAILURE);
	}

	if( signal( SIGINT, manejador) == SIG_ERR )
	{
		perror("Error en el manejador");
		exit(EXIT_FAILURE);
	}
}

int iniServidor( )
{
	int sockfd;
   	struct sockaddr_in direccion_servidor; //Estructura de la familia AF_INET, que almacena direccion

/*
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
 *  en arquitecturas little-endian se deben revertir los bytes
 *  INADDR_ANY - Se elige cualquier interfaz de entrada
 */
   	memset (&direccion_servidor, 0, sizeof (direccion_servidor));	//se limpia la estructura con ceros
   	direccion_servidor.sin_family 		= AF_INET;
   	direccion_servidor.sin_port 		= htons(PUERTO);
   	direccion_servidor.sin_addr.s_addr 	= INADDR_ANY;
/*
 *	Creacion de las estructuras necesarias para el manejo de un socket
 *  SOCK_STREAM - Protocolo orientado a conexión
 */
   	printf("Creando Socket ....\n");
   	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
   	}
/*
 *  bind - Se utiliza para unir un socket con una dirección de red determinada
 */
   	printf("Configurando socket ...\n");
   	if( bind(sockfd, (struct sockaddr *) &direccion_servidor, sizeof(direccion_servidor)) < 0 )
	{
		perror ("Ocurrio un problema al configurar el socket");
		exit(1);
   	}
/*
 *  listen - Marca al socket indicado por sockfd como un socket pasivo, esto es, como un socket
 *  que será usado para aceptar solicitudes de conexiones de entrada usando "accept"
 *  Habilita una cola asociada al socket para alojar peticiones de conector procedentes
 *  de los procesos clientes.
 */
   	printf ("Estableciendo la aceptacion de clientes...\n");
	if( listen(sockfd, COLA_CLIENTES) < 0 )
	{
		perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
		exit(1);
   	}
	   
	/* Retornamos el descriptor del socket */
	return sockfd;
}

void atiendeCliente( int cliente_sockfd, unsigned char *imagenGray, bmpInfoHeader *info )
{
    int tamImagen = info->width*info->height;
	/* Dirección de los datos que voy a enviar, así como el tamaño en bytes del
	dato o los datos que vamos a enviar, el tercer parámetro es el tamaño en bytes de la
	imagen, como es de tamaño unsigned char, entonces ocupa un byte, por lo que resulta 
	conveniente utilizar la función sizeof */
	printf("Enviando la estrutura info de la imagen... \n");
	if( write (cliente_sockfd, info, sizeof(bmpInfoHeader) ) < 0 )
	{
		perror("Ocurrio un problema en el envio de la imagen al cliente ");
		exit(EXIT_FAILURE);
	}
	printf("Enviando Imagen... \n");
	if( write (cliente_sockfd, imagenGray, tamImagen ) < 0 )
	{
		perror("Ocurrio un problema en el envio de la imagen al cliente ");
		exit(EXIT_FAILURE);
	}
//kill( getppid(), SIGUSR1 );
	close (cliente_sockfd);
	exit(0);
}

void manejador( int sig )
{
	int estado;
	pid_t pid;

	if( sig == SIGCHLD )
	{
		printf("Se recibio la señal SIGCHLD en el servidor\n");
		pid = wait(&estado);
		printf("Termino el proceso %d con estado: %d\n", pid, estado>>8);
	}else if( sig == SIGINT )
	{
		printf("Se recibio la señal SIGINT en el servidor\n");
	   	printf("Concluimos la ejecución de la aplicacion Servidor \n");
   		finPrograma = 1;
		
	}
}


void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height )
{
	unsigned char nivelGris;
	int indiceRGB, indiceGray;

	for( indiceGray = 0, indiceRGB = 0; indiceGray < (height*width); indiceGray++, indiceRGB += 3 )
	{
	//	nivelGris = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2]) / 3;
		nivelGris = (30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2]) / 100;
		// 0.30 = 30 / 100
		// 0.59 = 59 / 100
		// 0.11 = 11 / 100
		imagenGray[indiceGray] = nivelGris;
	}
}

unsigned char * reservarMemoria( uint32_t width, uint32_t height )
{
	unsigned char *imagen;

	imagen = (unsigned char *)malloc( width*height*sizeof(unsigned char) );
	if( imagen == NULL )
	{
		perror("Error al asignar memoria a la imagen");
		exit(EXIT_FAILURE);
	}

	return imagen;
}
