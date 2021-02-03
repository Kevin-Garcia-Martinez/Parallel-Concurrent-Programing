#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "imagen.h"
#include "proc.h"
#include "thread.h"
#include "defs.h"

/* Creamos la estructura de datos que compartirán todos los hilos */
parameters p;

int main(){

    register int nh; 
    int *hilo;
    int nhs[NUM_HILOS];
    /* Declaramos un arreglo de identificadores para todos los hilos que se crean */
    pthread_t tids[NUM_HILOS];
    
    /* Estructura para almacenar la información de la imagen BMP*/
    bmpInfoHeader info;
    /* Declaramos apuntadores unsigned_char */
    unsigned char *imagenRGB;

    /* Esta función regresará un apuntador unsigned_char donde estará contenido todo el 
    cuerpo de la imagen que se recupero en la lectura del archivo */
    imagenRGB = abrirBMP("huella1.bmp", &info );
    /* Imprimimos lo que se leyó de la imagen bmp */
    displayInfo( &info );
    /* Reservamos memoria para la imagen a escala de grises y la imagen filtrada */
    p.imagenGray     = reservarMemoria( info.width, info.height );
    p.imagenFiltrada = reservarMemoria( info.width, info.height ); 
    p.height         = info.height;
    p.width          = info.width;
    /* Rellenamos los bordes que tendrá la imagen al aplicar el filtro pasa bajas con 
    un pixel 255, es decir, con color blanco */
    memset( p.imagenFiltrada, 255, info.width * info.height);
    /* Pasamos de RGB a escala de grises */
    RGBToGray( imagenRGB, p.imagenGray, info.width, info.height );
    /* Llamamos la función para realizar el proceso de filtrado, en los hilos
    la memoria es compartida entre ellos  */
    for (nh = 0; nh < NUM_HILOS; nh++){  
        nhs[nh] = nh;
        pthread_create( &tids[nh], NULL, filtroPasaBajas, (void *)&nhs[nh]);
    }

    for ( nh = 0; nh < NUM_HILOS; nh++){
        pthread_join( tids[nh], (void**)&hilo);
        /* Imprimimos el contenido de la variable resultado con '*' */
        printf("El hilo termino su ejecución %d \n", *hilo);
    }

    /* Pasamaos la imagen filtrada (en escala de grises) a RGB */
    GrayToRGB( imagenRGB, p.imagenFiltrada, info.width, info.height);
    /* Escribimos el archivo de la imagen resultante */
    guardarBMP("huellaFPBG.bmp", &info, imagenRGB);
    /* Liberamos la memoria del apuntador de cada imagen */
    free( imagenRGB );
    free( p.imagenGray );
    free( p.imagenFiltrada );
    
    return 0;
    
}