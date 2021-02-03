#include <stdio.h>
#include <stdlib.h>
#include "imagen.h" 

/* Prototipos de las funciones */
void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);

unsigned char *reservarMemoria( uint32_t width, uint32_t height);

int main(){
    /* Estructura para almacenar la información de la imagen BMP*/
    bmpInfoHeader info;
    /* Declaramos un apuntador unsigned_char */
    unsigned char *imagenRGB;
    unsigned char *imagenGray;
    /* Esta función regresará un apuntador unsigned_char donde estará 
    contenido todo el cuerpo de la imagen que se recupero en la lectura
    del archivo */
    imagenRGB = abrirBMP("hoja.bmp", &info );
    /* Imprimimos lo que se leyó de la imagen bmp */
    displayInfo( &info );
    /* Reservamos memoria para la imagen a escala de grises */
    imagenGray = reservarMemoria(info.width, info.height);
    /* Vamos a realizar la conversión de color a nivel de gris */
    RGBToGray( imagenRGB, imagenGray, info.width, info.height );
    /* Aumentar el brillo de la imagen */
    /*
    255: Mientras más nos acerquemos a la componente blanca, más luminosidad le damos
    0: Negro
    */
    brilloImagen( imagenGray, info.width, info.height );

    
    /* Tenemos que hacer una función que guarde la imagen que esta en niveles de gris a RGB*/
    GrayToRGB( imagenRGB, imagenGray, info.width, info.height);
    /* Escribimos el archivo de la imagen resultante */
    guardarBMP("hoja-brillo.bmp", &info, imagenRGB);
    /* Liberamos la memoria del apuntador */
    free( imagenRGB );
    free( imagenGray );
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
/* FUNCIONES TRADICIONALES DEL RECORRIDO DE IMAGENES */

// void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height){
//     register int x, y;
//     int indiceGray, indiceRGB;
    
  
//     for( y=0; y<height; y++)
//         for( x=0; x<width; x++){
//             /* Indice en nivel de gris 1 byte por cada pixel en lugar de 3 bytes por pixel (RGB)*/
//             indiceGray = y*width + x;
//             /* indiceRGB solamente hace referencia a la componente Roja (R) del pixel */
//             indiceRGB  = indiceGray * 3;
//             /* En cada componente del RGB colocaremos el nivel de gris que calculamos anterioemente
//             al tener las 3 componente con el mismo valor ese pixel se guarda en niveles de gris*/
//             imagenRGB[indiceRGB]   = imagenGray[indiceGray];
//             imagenRGB[indiceRGB+1] = imagenGray[indiceGray];
//             imagenRGB[indiceRGB+2] = imagenGray[indiceGray];
//         }
// }


/* Parámetro que recibde:

imagenRGB: Imagen a color que se leyó.
imagenGray: Imagen a escala de grises, se recibe como un apuntador para guardar la información en esa variable sin necesidad de retornarla
width: Ancho de la imagen (coordenada x)
heigth: ALtura de la imagen (coordenada y)

*/
// void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height){
//     register int x, y;
//     int indiceGray, indiceRGB;
//     /* Este solamente utilizará un byte dentro de la imagen a escala de grises */
//     unsigned char nivelGris;
  
//     for( y=0; y<height; y++)
//         for( x=0; x<width; x++){
//             /* Indice en nivel de gris 1 byte por cada pixel en lugar de 3 bytes por pixel (RGB)*/
//             indiceGray = y*width + x;
//             /* indiceRGB solamente hace referencia a la componente Roja (R) del pixel */
//             indiceRGB  = indiceGray * 3;
//             /* Tomamos cada componente RGB de la imagenRGB y obtenemos su promedio, estamos
//             sumando la componente en Rojo (R) más componente en Verde (G) más la componente en Azul (B)  */
//             /* *Ponderación por promedio, 0.3 para cada una (equitativa )/
//             nivelGris = ( imagenRGB[indiceRGB] + imagenRGB[indiceRGB+1] + imagenRGB[indiceRGB+2] ) / 3; 
//             */ 
//             /* Ponderacion basada en la luminosidad de la imagen */
//             /*
//               0.30 = 30/100
//               0.59 = 59/100
//               0.11 = 11/100 
//               Para no utilizar aritmética de punto flotante utilizamos esta forma para seguir trabajando con
//               aritmética de punto fijo
//             */
//             nivelGris = ( 30*imagenRGB[indiceRGB] + 59*imagenRGB[indiceRGB+1] + 11*imagenRGB[indiceRGB+2] ) / 100;
//             imagenGray[indiceGray] = nivelGris;
//         }

// }


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