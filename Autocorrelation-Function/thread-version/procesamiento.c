/** @brief procesos.c, se definen las funciones que
 *  serán ejecutadas por hilos lanzados.  
 */ 

#include <stdio.h>
#include <math.h>
#include "defs.h"
 
void generateSinFunction ( float datos [] ){
     float f = 1000, fs = 45000;
     /* El iterador 'n' será un registro para que el compilador
        y el S.O accedan más rápido a esta variable */
     register int n;

     /* Ciclo para generar las muestras */
     for (n = 0; n < NUM_MUESTRAS; n++)
     {
         datos[n] = sinf(2*n*M_PI*f/fs);
     }
     
}

void generateHannFunction ( float datos [] ){
     /* El iterador 'n' será un registro para que el compilador
        y el S.O accedan más rápido a esta variable */
     register int n;

     /* Ciclo para generar las muestras */
     for (n = 0; n < NUM_MUESTRAS; n++)
     {
         datos[n] = pow(sinf((n*M_PI)/NUM_DATOS), 2);
     }
     
}

