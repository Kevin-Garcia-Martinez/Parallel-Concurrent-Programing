/** @brief procesos.h, definición de las funciones
 */
#ifndef PROCESS_H
/* directivas del preprocesador */
#define PROCESS_H

#include "defs.h"

/* protocolos de las funciones */
void proceso_padre( int pipefd[NUM_PROC][2]);
void proceso_hijo( int np, int pipefd[], int *datos );

#endif
