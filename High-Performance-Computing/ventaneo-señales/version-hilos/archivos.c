/** @brief archivos.c, este programa
 *  guarda un arreglo de datos en un
 *  archivo.
 */ 

#include<stdio.h>
#include<stdlib.h>
#include "defs.h"

void guardaDatosArchivo( float datos [], char *nombre ){
    
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

    for (n = 0; n < NUM_MUESTRAS; n++){
         fprintf(ap_arch, "%f \n", datos[n]);
     }

     /* Cerramos el archivo */
     fclose( ap_arch );

}

void leerDatosArchivo(float datos[], char *nombre ){
     FILE *ap_arch;
     char renglon[6];
     char filename[30];
     sprintf(filename, "%s.dat", nombre);
     register int n;
     /* Se abre el archivo en modo lectura  */
     ap_arch = fopen( filename, "r" );
     /* Verificamos que el archivo se pueda abrir */
     if ( ap_arch == NULL ){
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
     }
     n = 0;
     while(fgets(renglon, 6 , ap_arch ) != NULL ) {
        datos[n] = (float) atoi(renglon);
        datos[n] -= 2200;
        n++;
     }
     

    fclose( ap_arch );

}