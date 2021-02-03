/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "defs.h"
/* Estructura de datos que será común en todos los hilos */
extern parameters p;

/* Colocaremos la función para realizar el segmentado de la imagen, es decir, detectaremos
   los cambios bruscos de intensidad, como primero convertimos la imagen a una escala de 
   grises, es fácil detectar los cambios bruscos de color negro -> blanco:

    - La función recibirá la imagen en escala de grises
    - Un segundo arreglo donde se quedará el resultado de aplicar la función de detección
      de bordes.
     
    Retorno: La detección de bordes hace que la imagen se ve en blanco y negro, pero esta 
    tiene bien definidos los bordes. Los bordes en la imagen resultante se verán de color
    blanco y donde no se detecten bordes veremos el color negro. 
*/

void * gradienteSobel( void *arg ){

    int register x, y, ym, xm;
    int indicem, indicei, convfila, convcolumna;
    /* Para la detección de bordes utilizaremos un gradiente fila y un gradiente columna,
      así mismo haremos uso de la constante de Sobel (K = 2) como nuestro factor.
      Al tener dos gradientes ahora tendremos que calcular la convolución de ambos y el 
      resultado se conoce como vector fila al cual le sacaremos su magnitud (modulo) y
      ese será el valor del pixel que tenemos que poner en la posición determinada de 
      la imagen.  */

    int gradienteFila[DIMASK*DIMASK] = {
        1, 0, -1,
        2, 0, -2,
        1, 0, -1,
    };

    int gradienteColumna[DIMASK*DIMASK] = {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1,
    };

    /* El número de hilo que entrará a ejecutar esta función ahora será el núcleo */
    int nucleo = *(int *)arg;
    /* Calculo del tamaño de bloque ( Cuantas filas le tocan a cada hilo ) */ 
    int elemBloque = p.height / NUM_HILOS;
    /* Calculamos el inicio de bloque de cada hilo*/
    int iniBloque  = nucleo * elemBloque;
    /* Calculamos el fin de bloque de cada hilo*/
    int finBloque;
    
    if (nucleo == NUM_HILOS-1 ){
        /* Calculamos el residuo */
        int res, add = 0;
        res = p.height%NUM_HILOS;
        
        /* Si hay residuo */
        if ( res )
            add = iniBloque + elemBloque + 1 + res;
        else
            add = iniBloque + elemBloque + 1;

        finBloque  = add - DIMASK;
    }
    else{
        finBloque  = iniBloque + elemBloque;
    }    
    
    /* Variable del ciclo for */
    printf("Hilo %d en ejecución \n", nucleo);
    
    /* Paralelización por Bloques */
    
    /* Lo primero que haremos es recorrer toda la imagen en escala de grises */
    for ( y = iniBloque; y < finBloque; y++)    /* Filas */
        for ( x = 0; x <= p.width-DIMASK ; x++) /* Columnas */
        {
            /* EL índice de la máscara siempre comieza en cero, ya que este se recorre de manera lineal,
            es decir, recorreremos las posiciones 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 cada vez que hagamos la convolusión  */
            indicem     = 0;
            convfila    = 0;
            convcolumna = 0;
            /* Desplazarme sobre la subimagen, la cual es la máscara encimada en la imagen a escala de grises*/
            for (ym = 0; ym < DIMASK ; ym++) /* Filas de la subimagen */ 
                for ( xm = 0; xm < DIMASK; xm++) /* Columnas de la subimagen */ 
                {
                    /* Calculamos el índice de la imagen */
                    indicei = ( y + ym ) * p.width + ( x + xm );
                    /* Calculamos la convolusión de los pixeles de la imagen con los gradientes, es decir,
                    tenemos que realizar dos convoluciones, una por cada gradiente, esto me dará como 
                    resultado el vector fila que necesitamos [ derivada parcial de x, derivada parcial de y ]*/
                    convfila    += p.imagenGray[indicei] * gradienteFila[indicem];
                    convcolumna += p.imagenGray[indicei] * gradienteColumna[indicem++];
                }
            /* Aplicamos el factor para asegurarnos que no nos excedamos el máximo nivel de gris que es 255*/ 
            convfila    = convfila    >>2; /* Hacer un corrimiento de 2 bits a la derecha es lo mismo que dividir entre 4*/
            convcolumna = convcolumna >>2;
            /* Necesitamos obtener la magnitud del vector fila formado por convfila y convcolumna */
            unsigned char magnitud_vector = (unsigned char) sqrt( (convfila*convfila) + (convcolumna*convcolumna) );
            /* El resultado de la magnitud de las convolusiones se coloca en medio de la región de las dos imágenes que se están convolusionando */
            indicei = ( y+1 )* p.width + ( x+1 );
            /* Imagen Segmentada, es unsined char porque es un valor de 8 bits*/
            p.imagenSegmentada[indicei] = magnitud_vector;
        }
        
    /* Retornaremos el resultado de la operación que realizó cada uno de los hilos */
    pthread_exit( arg );
}
