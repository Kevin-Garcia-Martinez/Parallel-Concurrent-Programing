#include <stdint.h>
#include <stdio.h>

#ifndef IMAGEN_H
#define IMAGEN_H

typedef struct bmpFileHeader
{
  /* 2 bytes de identificación */
  uint32_t size;        /* Tamaño del archivo 4 bytes*/
  uint16_t resv1;       /* Reservado 2 bytes */
  uint16_t resv2;       /* Reservado 2 bytes*/
  uint32_t offset;      /* Offset hasta hasta los datos de imagen 4 bytes*/
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  uint32_t headersize;      /* Tamaño de la cabecera */
  uint32_t width;           /* Ancho (cantidad de pixeles) */
  uint32_t height;          /* Alto (cantidad de pixeles) */
  uint16_t planes;          /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* Bits por pixel */
  uint32_t compress;        /* Compresión */
  uint32_t imgsize;         /* tamaño de los datos de imagen */
  uint32_t bpmx;            /* Resolución X en bits por metro */
  uint32_t bpmy;            /* Resolución Y en bits por metro */
  uint32_t colors;          /* Colores usados en la paleta */
  uint32_t imxtcolors;      /* Colores importantes. 0 si son todos */
} bmpInfoHeader;


unsigned char *abrirBMP(char *filename, bmpInfoHeader *bInfoHeader);
void guardarBMP(char *filename, bmpInfoHeader *info, unsigned char *imgdata);
void displayInfo(bmpInfoHeader *info);

#endif