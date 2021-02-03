/** @brief procesos.h, definición de las funciones
 */
#ifndef PROCESS_H
/* directivas del preprocesador */
#define PROCESS_H

#include "defs.h"

/* Cada proceso hijo recibirá la tubería que le corresponde */
void proceso_hijo( int np, int pipefd[]);
/* El proceso padre recibirá un arreglo de tuberías  */
void proceso_padre( int pipefd[NUM_PROC][2] );

#endif

