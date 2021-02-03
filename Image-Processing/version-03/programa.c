#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define WIDTH  8000
#define HEIGHT 7000

int * reservarMemoria( void );
void sumaMatrices( int *A, int *B, int *C );
void llenarArreglo( int *matriz );

int main( )
{
	int *A, *B, *C;

	A = reservarMemoria();
	B = reservarMemoria();
	C = reservarMemoria();

	llenarArreglo( A );
	llenarArreglo( B );
	llenarArreglo( C );

	sumaMatrices( A, B, C );

	free( A );
	free( B );
	free( C );

	return 0;
}

void llenarArreglo( int *matriz )
{
	register int x, y;
	int indice;

	for( y = 0; y < HEIGHT; y++ )
	{
		for( x = 0; x < WIDTH; x++ )
		{
			indice = (y*WIDTH + x);
			matriz[indice] = rand() % 8192;
		}
	}
}


void sumaMatrices( int *A, int *B, int *C )
{
	register int x, y;
	int indice;
    /* Realizamos la suma de matrices */
	for ( x = 0; x < WIDTH; x++)
		for ( y = 0; y < HEIGHT; y++)
		{
			/* El elemento de la Arquitecutra que tiene que ver con
			este fenómeno es la Cache y la RAM*/
			indice = y*WIDTH + x;
			C[indice] = A[indice] + B[indice];
		}

}

int * reservarMemoria( void )
{
	int *mem;

	mem = (int *)malloc( sizeof(int) * WIDTH * HEIGHT );
	if( !mem )
	{
		perror("Error de asignación de memoria");
		exit(EXIT_FAILURE);
	}

	return mem;
}

/* Con el comando time ./image podemos ver cuando se tarda en ejecutarse nuestro programa */