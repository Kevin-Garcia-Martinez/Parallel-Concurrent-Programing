//Multiplicación de vectores

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Prototipos de las funciones*/
void pedirDatos(int *escalar, int *elementos);
void capturarDatos(int *vector, int elementos);
void vectorPorEscalar(int *vector, int elementos, int escalar);
void imprimirVector( int *vector, int elementos );

int main() {
	int escalar, elementos;
    /* Paso por referencia, el valor asignado se queda almacenado en esa variable */
	pedirDatos(&escalar, &elementos);
    /* Creando el arreglo de acuerdo al número de elementos */
    int vector [elementos];
    /* Llamas a tu función para capturar los datos del vector, lo pasas por referencia */
    capturarDatos( vector, elementos );
    /* Imprimimos el vector original */
    printf("Vector Origininal: ");
    imprimirVector( vector, elementos );
    /* Realizamos la multiplicación del escalar por el vector */
    vectorPorEscalar( vector, elementos, escalar); 
    /* Imprimimos el vector resultado */
    printf("Vector por Escalar: ");
    imprimirVector( vector, elementos );

	//system ("pause"); 
    return 0;
}

void pedirDatos(int * escalar, int * elementos){
	printf("\n1. Multipliplicacion de un escalar por un vector.");
			
	printf("\n\nIngrese el numero de elementos que tendra el vector: ");
	scanf("%d", elementos);
			
	printf("\nIngrese el escalar: ");
	scanf("%d", escalar);	

}

void capturarDatos(int *vector, int elementos){
	printf("\nIngrese los elementos del vector.\n");
	int i;	
	
    for(i=0; i<elementos; i++){
		printf(" [%d]= ", i); 
		scanf("%d", & vector[i]);
        printf("\n");
	}
	
}

void vectorPorEscalar(int *vector, int elementos, int escalar){	
	int resultado = 0;
    int i;
    for(i=0; i<elementos; i++)
        vector[i]*=escalar;
}

void imprimirVector( int *vector, int elementos ){
    int i;
    for ( i = 0; i < elementos; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");    
}
