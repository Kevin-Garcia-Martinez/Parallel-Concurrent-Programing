
/** @brief procesos.c, se definen los algoritmos que
 *  serán ejecutados por los procesos creados.  
 */ 

#include<stdio.h>
#include "defs.h"

int buscarMayor(int *datos){
    int mayor = datos[0];
    register int i;
    for ( i = 1; i < N; i++)
    {
        if(datos[i]>mayor){
            mayor = datos[i];
        }
    }

    return mayor;
}

int buscarMenor(int *datos){
    register int i;
    int menor = datos[0];
    for ( i = 1; i < N; i++)
    {
        if(datos[i]<menor){
            menor = datos[i];
        }
    }
    return menor;
}

float obtenerPromedio(int *datos){
    register int i;
    float suma = 0;
     
    for ( i = 0; i < N; i++)
    {
        suma += datos[i];
    }

    return suma/N; 
}

int buscarPares(int *datos){
    register int i;
    int pares = 0;
    for ( i = 0; i < N; i++)
    {
        if (datos[i]%2==0){
            pares+=1;
        }
    }

    return pares;
    
}