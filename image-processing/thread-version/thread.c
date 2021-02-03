/** @brief hilos.c, gestionada cada una de las
 *  tareas que realizarán los hilos que se lanzen. 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "defs.h"
/* Estructura de datos que será común en todos los hilos */
extern parameters p;

/* Colocaremos la función para realizar el filtro pasabajas, es decir, las
   frecuencias bajas que tendremos en el lóbulo principal de la función Gaussiana:

    - La función recibirá la imagen en escala de grises
    - Un segundo arreglo donde se quedará el resultado de aplicar la función de filtrado
      a la imagen en escala de grises.
     
    Retorno: El filtro provoca un suavizado en la imagen, es decir, la vemos difuminada 
    debido a que estamos eliminando las altas frecuencias de los bordes de la imagen.
*/

void * filtroPasaBajas( void *arg ){

    int register x, y, ym, xm;
    int indicem, indicei, conv, factor = 330;
   /* Filtro pasabajas donde los coeficientes se obtuvieron de una función Gaussiana
   para que la respuesta al impulsp no tenga los lóbulos laterales como los tenía la función Sampling */

    int kernelGaussiano[DIMASK*DIMASK] = {
        1,  4,   7,   4, 1,
        4,  20, 33,  20, 4,
        7,  33, 54,  33, 7,
        4,  20, 33,  20, 4,
        1,   4,  7,   4, 1,
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
            indicem = 0;
            conv = 0;
            /* Desplazarme sobre la subimagen, la cual es la máscara encimada en la imagen a escala de grises*/
            for (ym = 0; ym < DIMASK ; ym++) /* Filas de la subimagen */ 
                for ( xm = 0; xm < DIMASK; xm++) /* Columnas de la subimagen */ 
                {
                    /* Calculamos el índice de la imagen */
                    indicei = ( y + ym ) * p.width + ( x + xm );
                    /* Calculamos la convolusión de los pixeles de la imagen con la máscara*/
                    conv +=  p.imagenGray[indicei] * kernelGaussiano[indicem++];
                }
            /* Aplicamos el factor para asegurarnos que no excedamos el máximo nivel de gris que es 255*/ 
            conv = conv / factor;
            /* El resultado de la convolusión se coloca en medio de la región de las dos imágenes que se están convolusionando */
            indicei = (y+2)* p.width + (x+2);
            p.imagenFiltrada[indicei] = conv;
        }
        
    /* Retornaremos el resultado de la operación que realizó cada uno de los hilos */
    pthread_exit( arg );
}
