#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "imagen.h"

#define PUERTO 5000
#define TAM_BUFFER 100
/* Aquí se coloca la dirección IP de la tarjeta RASP BERRY Pi*/
#define DIR_IP "127.0.0.1"

unsigned char * reservarMemoria( int tamImagen );
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void recibirImagen( int sockfd, unsigned char *imagen, int tamImagen );

int main(int argc, char **argv)
{
	bmpInfoHeader info;
    int sockfd;
	struct sockaddr_in direccion_servidor;
	unsigned char *imagen, *imagenRGB;

/*
 *	AF_INET - Protocolo de internet IPV4
 *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
 *  en arquitecturas little-endian se deben revertir los bytes
 */	
	memset (&direccion_servidor, 0, sizeof (direccion_servidor));
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_port = htons(PUERTO);
/*	
 *	inet_pton - Convierte direcciones de texto IPv4 en forma binaria
 */	
	if( inet_pton(AF_INET, DIR_IP, &direccion_servidor.sin_addr) <= 0 )
	{
		perror("Ocurrio un error al momento de asignar la direccion IP");
		exit(1);
	}
/*
 *	Creacion de las estructuras necesarias para el manejo de un socket
 *  SOCK_STREAM - Protocolo para sockets orientado a conexión
 */
	printf("Creando Socket ....\n");
	if( (sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
	}
/*
 *	Inicia el establecimiento de una conexion mediante una apertura
 *	activa con el servidor
 *  connect - ES BLOQUEANTE
 */
	printf ("Estableciendo conexion ...\n");
	if( connect(sockfd, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor) ) < 0) 
	{
		perror ("Ocurrio un problema al establecer la conexion");
		exit(1);
	}
/*
 *	Inicia la transferencia de datos entre cliente y servidor
 */

  /* Antes de recibir la imagen, primero debemos de enviar el tamaño de la imagen,
  para esto recibiremos la cabecera completa de la imagen. */
    
	printf ("Recibiendo la estructura info de la imagen del servidor ...\n");
	if (read (sockfd, &info, sizeof(bmpInfoHeader)) < 0)
	{
		perror ("Ocurrio algun problema al recibir del tamaño de la imagen del servidor");
		exit(1);
	}
	/* Le reservamos memoria a la imagen una vez que ya recibimos el tamaño que ocupara esta */
    
    imagen    = reservarMemoria( info.width*info.height );
	imagenRGB = reservarMemoria( info.width*info.height*3 );
	
	printf ("Recibiendo la imagen del servidor ...\n");
	/* Recibe el arreglo en donde se recibira la imagen y la cantidad de bytes a leer*/
	recibirImagen( sockfd, imagen, info.width*info.height );
     
	/* Trabajar con imágenes en escala de grises es más eficiente porque
	utilizan menos espacio, es tres veces menos el espacio utilizado*/
	GrayToRGB( imagenRGB, imagen, info.width, info.height);
	guardarBMP( "huella1.bmp", &info, imagenRGB );

	printf("El servidor recibio la imagen\n");
	printf ("Cerrando la aplicacion cliente\n");
	/* Guardamos la imagen */

/*
 *	Cierre de la conexion
 */
	close(sockfd);
	free(imagen);
	free(imagenRGB);

	return 0;
}
	
unsigned char * reservarMemoria( int tamImagen )
{
	unsigned char *imagen;

	imagen = (unsigned char *)malloc( tamImagen*sizeof(unsigned char) );
	if( imagen == NULL )
	{
		perror("Error al asignar memoria a la imagen");
		exit(EXIT_FAILURE);
	}

	return imagen;
}


void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height){
    
    int indiceGray, indiceRGB;
    
    for( indiceGray = 0, indiceRGB = 0; indiceGray<(height*width); indiceGray++, indiceRGB+=3 ){
            /* En cada componente del RGB colocaremos el nivel de gris que calculamos anterioemente
            al tener las 3 componente con el mismo valor ese pixel se guarda en niveles de gris*/
            imagenRGB[indiceRGB]   = imagenGray[indiceGray];
            imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
            imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
    }
}


/* Cuando del lado del servidor se utiliza la función write el armado de las tramas de 
mtu se hace automátimacamente, es decir, se envían todos los bytes del archivo grande 
(más grande que el tamaño del mtu por defecto) hacia el cliente. 

Pero  a la hora de la recepción (del lado del cliente)
*/


/* En esta función necesitamos el decriptor del socket */
void recibirImagen( int sockfd, unsigned char *imagen, int tamImagen )
{
	int bytesRecibidos;
	/* La función read regresa el número de bytes que se leyeron, la función read
	los bytes dentro de una trama mtu, la cual tenemos configurada por defecto con un
	tamaño de 1500 bytes */
	while( tamImagen > 0){ /* Si el tamaño de la imagen es mayor a cero, significa que aún siguen habiendo bytes sin leer*/
		bytesRecibidos = read (sockfd, imagen, tamImagen);
    
		if( bytesRecibidos < 0)
		{
			perror ("Ocurrio algun problema al recibir imagen del servidor");
			exit(1);
		}
		printf("Bytes recibidos: %d\n", bytesRecibidos);
		/* Actualizamos el tamaño de la imagen a leer, ya que ahora le restaremos los bytes que ya se leyeron*/
		tamImagen -= bytesRecibidos;
		/* El apuntador de la imagen también tiene que desplazarse, para que ahora
		el nuevo bloque de datos de la imagen se escriba en la siguiente dirección de memoria */
		imagen += bytesRecibidos;
	}
	

}