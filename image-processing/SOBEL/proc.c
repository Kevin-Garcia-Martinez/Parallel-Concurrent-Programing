#include <stdio.h>
#include <stdlib.h>
#include "proc.h"

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