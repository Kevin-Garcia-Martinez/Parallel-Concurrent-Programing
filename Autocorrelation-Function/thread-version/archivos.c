/** @brief archivos.c, este programa
 *  guarda un arreglo de datos en un
 *  archivo.
 */ 

#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "defs.h"

void storeDataInFile ( float datos [], char *nombre ){
    
    FILE *ap_arch;
    char filename[30];
    sprintf(filename, "%s.dat", nombre);
    register int n;
 
    /* Si el archivo no existe este se crea */
    ap_arch = fopen( filename, "w" );
    
    /* Verificamos que el archivo se pueda abrir */
    if ( ap_arch == NULL ){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (n = 0; n < NUM_DATOS; n++){
         fprintf(ap_arch, "%f \n", datos[n]);
    }

     /* Cerramos el archivo */
     fclose( ap_arch );

}

void readDataFromFile ( float datos[], char *nombre ){
     
     FILE *ap_arch;
     register int n;

     char filename[30];
     sprintf(filename, "%s.dat", nombre);
     
     /* Se abre el archivo en modo lectura  */
     ap_arch = fopen( filename, "r" );
     /* Verificamos que el archivo se pueda abrir */
     if ( ap_arch == NULL ){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
     }
     
     for (n = 0; n < NUM_DATOS; n++){
         fscanf(ap_arch, "%f \n", &datos[n] );
         /* Eliminamos el offset */
         datos[n] -= 2200;
     }

    fclose( ap_arch );

}