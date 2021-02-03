#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#define NUM_PROC 2
#define N 32 // 32 datos
#define EVER 1
void proceso_hijo( int np, int *datos);
void proceso_padre( );
void llenarArreglo( int *);
int * reservarMemoria();
void imprimirArreglo(int *datos);
int buscarMayor(int *datos);
int buscarMenor(int *datos);
/* 
.h no se pone en el archivo Makefile, lo único que se compila es el .c
la directiva include le indica al compilador que pegue lo que hay en el archivo.h en el archivo .c
2 procesos ejecutaran dos algoritmos diferentes
pero trabajando simultaneamente, es decir, haciendo la
parte de paralelización. 
1 buscará el número más grande en nuestro arreglo de datos.
y otro buscará el número más pequeño en nuestro arreglo de datos

Tarea: 
Hacer un programa que genere 4 procesos hijos y un conjunto de 128 valores enteros 
y cada proceso ejecutará un algoritmo de los siguientes:
1. Buscar el mayor del conjunto de valores
2. Buscar el menor del conjunto de valores
3. Calcular el promedio del conjunto de valores 
4. Obtener cuantos números pares existen en el conjunto de datos

Generar un proyecto con los siguientes archivos:
1. principal.c: este archivo debe de contener a la función main 
2. procesos.c: este archivo debe tener las funciones del proceso hijo y procesos padre
# Código del proceso hijo y padre
3. procesamiento.c: este archivo debe tener las funciones de los algoritmos.
4. helper.c: este archivo debe tener las funciones de reservarMemoria, llenarArreglo e imprimir Arreglo
5. Definir los archivos .h correspondientes (procesos.h declaraciones de las funciones de proceso hijo y padre
procesamiento.h declaraciones de las funciones buscarMayor, buscarMenor, obtenerPromedio. 
helper.h estarán declaras las funciones de llenarArreglo y en defs.h N NUM_PROC )
6. Archivo Makefile completo

*/

int main(){
    /* Cada vez que corremos este programa, el Sistema Operativo
       le asigna un valor distinto al pid del proceso padre.*/
    int *datos;
    pid_t pid;
    int status;
    register int np;
    /* Generamos la semilla, esta semilla la toma como valor la función rand */
    srand(getpid()); 
    /* Reservamos memoria */ 
    datos = reservarMemoria();
    /* Llenamos el arreglo con números pdseudoaleatorios mediante un paso por referencia */
    llenarArreglo( datos ); /* Paso por referencia del arreglo al cual le reservamos memoria*/
    /* Imprimos los datos guardados en el arreglo */
    imprimirArreglo( datos );
    /* Generamos procesos hijos */
    for (np = 0; np < NUM_PROC; np++)
    {
        pid = fork();
        if( pid == -1){
            perror("Error al crear el proceso \n");
            exit(EXIT_FAILURE);
        } 
        /* Código del proceso hijo */
        if( !pid){
            /* Le pasamos el número de proceso que se esta ejecutando */
            proceso_hijo(np, datos);
        }
        
    }

    /* El pade continuará ejecutando  lo que hay después del ciclo for*/
    proceso_padre();
    /* Liberamos la memoria asignada */
    free(datos);

    return 0;
}

void imprimirArreglo(int *datos){
    register int i;
    for ( i = 0; i < N; i++){
        if(!(i%16)){
            printf("\n");
        }
        /* %3d imprime los números como si fueran de
           3 caracteres de longitud.   */
        printf("%3d ", datos[i]);
    }
    printf("\n");
    
}

void llenarArreglo( int *datos){
    register int i; 
    /* Llenamos nuestro arreglo */
    for(i=0; i<N;i++){
        datos[i] = rand()%256;
    }
}

int *reservarMemoria(){
    int * mem;
    /* malloc reserva una cantidad de bytes y devuelve un apuntador de tipo void*/
    mem = (int *) malloc( N*sizeof(int) );
    /* Retornamos la memoria si es que se asignó correctamente*/
    if(!mem){ // Si es nulo
        perror("Error al asignar memoria \n");
        exit(EXIT_FAILURE);
    }

    return mem;
}

/* Terminamos la ejecución del proceso hijo */
void proceso_hijo( int np, int *datos){
    int mayor, menor;
    /* con np podemos asignar tareas a los procesos hijos que creamos en el proceso padre*/
    if(np==0){
        mayor = buscarMayor( datos );
        exit(mayor); // Retornamos al proceso padre el número mayor de nuestro arreglo de datos
    }
    // Si es el proceso hijo 1 haz esto
    else if(np==1){
        menor = buscarMenor( datos );
        // Retornamos al proceso padre el número menor de nuestro arreglo de datos
        exit(menor);
    }

    /* Devolvemos el número de proceso hijo que está terminando su ejecución */
    /* Terminamos la ejecución del código, es decir, ya no van a continuar ejecutando más código*/
    exit(np);
}

/* Liberaremos a los procesos hijos que se encuentran en estado Zombi*/
void proceso_padre( ){
    pid_t pid;
    /* Variable para recuperar los datos que mandan los procesos hijos mediante la función exit() */
    int estado;
    register int np;
    /* Pasamos a wait una dirección de memoria para que en esta devuelva
    el valor que regreso el proceso hijo en la función exit() */
    for( np=0; np<NUM_PROC; np++){
        pid = wait(&estado);
        printf("Proceso hijo con pid: %d y valor de retorno: %d\n", pid, estado>>8);
    }

}

/*
Paso por valor: Si lo enviamos por valor, el valor de la variable que enviamos
a la función no se ve modificado, su valor queda igual a cómo se envio.
Paso por referencia: Significa pasar la dirección donde esta úbicada la variable
y se hace cuando queremos modificar el contenido de la variable que estamos enviando 
a la función. 
*/
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