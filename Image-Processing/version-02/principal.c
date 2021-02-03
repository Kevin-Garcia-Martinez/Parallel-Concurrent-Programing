#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imagen.h"
/* El tamaño del kernel será de 3x3*/
#define DIMASK 3

/* Prototipos de las funciones */
void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);
void filtroPB(unsigned char *imagenG, unsigned char *imagenF ,uint32_t width, uint32_t height);

unsigned char *reservarMemoria( uint32_t width, uint32_t height);

int main(){
    /* Estructura para almacenar la información de la imagen BMP*/
    bmpInfoHeader info;
    /* Declaramos un apuntador unsigned_char */
    unsigned char *imagenRGB;
    unsigned char *imagenGray;
    unsigned char *imagenFiltrada;

    /* Esta función regresará un apuntador unsigned_char donde estará 
    contenido todo el cuerpo de la imagen que se recupero en la lectura
    del archivo */
    imagenRGB = abrirBMP("linux.bmp", &info );
    /* Imprimimos lo que se leyó de la imagen bmp */
    displayInfo( &info );
    /* Reservamos memoria para la imagen a escala de grises */
    imagenGray = reservarMemoria(info.width, info.height);
    imagenFiltrada = reservarMemoria(info.width, info.height);
    /* Valor con el que queremos llenar esa dirección de memoria
    Cuántos bytes de la imagen filtrada vamos a llenar con 255, es decir, con el pixel de color blanco  */
    memset( imagenFiltrada, 255, info.width * info.height);

    /* Vamos a realizar la conversión de color a nivel de gris */
    RGBToGray( imagenRGB, imagenGray, info.width, info.height );
    
    /* Colocaremos la función para realizar el filtro pasabajas, es decir, las
    frecuencias bajas en el lóbulo principal de la función Sampling
    
    - La función recibirá la imagen en escala de grises
    - Un segundo arreglo donde se quedará el resultado del filtrado
     El filtro ocasiona que haya un suavizado en la imagen, es decir, la vemos difuminada 
     porque estamos eliminando las altas frecuencias de los bordes */
    filtroPB( imagenGray, imagenFiltrada, info.width, info.height );
    /* Tenemos que hacer una función que guarde la imagen que esta en niveles de gris a RGB*/
    GrayToRGB( imagenRGB, imagenFiltrada, info.width, info.height);
    /* Escribimos el archivo de la imagen resultante */
    guardarBMP("linuxFiltrada.bmp", &info, imagenRGB);
    /* Liberamos la memoria del apuntador de cada imagen */
    free( imagenRGB );
    free( imagenGray );
    free(imagenFiltrada);
    
    return 0;
}

void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
    register int indiceGray;
    int pixel;

    /* *Haremos un recorrido lineal de todos los pixeles porque le sumaremos una constante
    que aumentará el brillo en la imagen */
    for( indiceGray = 0; indiceGray<(height*width); indiceGray++){
        pixel = imagenGray[indiceGray] + 70;
        /* Si el pixel es mayor que 255, entonces le dejamos como máximo 255, caso contrario, le asignamos el valor que se obtuvo*/
        imagenGray[indiceGray] = ( pixel > 255 )? 255: (unsigned char)pixel; 
    }
}

/* Recibe cuantos pixeles se van a utilizar para la imagen  */
unsigned char *reservarMemoria( uint32_t width, uint32_t height){
    unsigned char *imagen;
    imagen = (unsigned char *) malloc( width*height*sizeof(unsigned char));
    /* Verificamos que se haya reservado correctamente la memoria para la imagen */
    if( imagen == NULL ){
        perror(" Error al asignar memoria a la imagen");
        exit(EXIT_FAILURE);
    }
    
    return imagen;
}

void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height){
    
    int indiceGray, indiceRGB;
    /* Este solamente utilizará un byte dentro de la imagen a escala de grises */
    unsigned char nivelGris;
    /* Solamente necesitamos un ciclo for, ya que recorreremos los pixeles de forma lineal  */
    for( indiceGray = 0, indiceRGB = 0; indiceGray<(height*width); indiceGray++, indiceRGB+=3 ){
        /* Ponderación basada en la luminisidad de la imagen */
        nivelGris = ( 30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2] ) / 100;
        imagenGray[indiceGray] = nivelGris;
    }

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

/* Función para filtrar una imagen, el resultado se quedará almacenado
en otro arreglo para conservar la imagen original */
void filtroPB(unsigned char *imagenG, unsigned char *imagenF ,uint32_t width, uint32_t height)
{
    int register x, y, ym, xm;
    int indicem, indicei, conv;

    /* Kernel/Máscara de  3x3, generalmente es impar para poder ubicar el pixel del centro en la imagen 
    es decir, este quede en medio cuando la máscara se encime con la imagen en escala de grises */
    int mascara[DIMASK*DIMASK] = {
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    };

    /* Lo primero que haremos es recorrer toda la imagen en escala de grises */
    for ( y = 0; y <= height-DIMASK; y++) /* Filas */
        for ( x = 0; x <= width-DIMASK ; x++) /* Columnas */
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
                    indicei = (y+ym)*width + (x+xm);
                    /* Calculamos la convolusión de los pixeles de la imagen con la máscara*/
                    conv += imagenG[indicei] * mascara[indicem++];
                }
            /* Aplicamos el factor para asegurarnos que no excedamos el máximo nivel de gris que es 255*/ 
            conv = conv / 9;
            /* El resultado de la convolusión se coloca en medio de la región de las dos imágenes que se están convolusionando */
            indicei = (y+1)*width + (x+1);
            imagenF[indicei] = conv;
        }

}