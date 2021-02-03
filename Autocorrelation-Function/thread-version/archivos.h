/** @brief archivos.h, en este archivo sólo se encuentran
 *  declaraciones y definiciones, nunca código. 
 */

/* Le indicamos al compilador que si no esta definido archivos.h, entonces 
   que lo defina, y así el compilador sólo lo compilará una vez */

#ifndef ARCHIVOS_H
#define ARCHIVOS_H

/* prototipos de las funciones */
void storeDataInFile  ( float datos[], char *nombre );
void readDataFromFile ( float datos[], char *nombre );

#endif