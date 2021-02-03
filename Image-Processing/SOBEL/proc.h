/** @brief procesamiento.h, se declaran los prototipos de las funciones
 */

#ifndef PROC_H
/* Directivas del preprocesador */
#define PROC_H

#include <stdint.h>

/* Prototipos de las funciones */
void RGBToGray( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GrayToRGB( unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void brilloImagen(unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria( uint32_t width, uint32_t height);


#endif