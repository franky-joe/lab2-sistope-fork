//-----LABORATORIO 2 -------//
//ARCHIVO PRINCIPAL PARA EL PROGRAMA
/*RAMO: SISTEMAS OPERATIVOS*/
/*AUTORES: FELIPE CORNEJO - CARLOS RETAMALES*/
/*AYUDANTE: BENJANMIN MUÑOZ*/

#include <ctype.h>  //getopt
#include <stdio.h>  //entrada y salida
#include <stdlib.h> //memoria dinamica
#include <string.h> //manejo de string
#include <unistd.h> //Uso de fork(), pipes(), entre otros
#include <sys/wait.h> //Constantes simbólicas (waitpid(), wait(), etc)
#include <sys/types.h> //Uso de tipos de datos ( pid_t )

//Definicion para las Pipes
#define LECTURA 0
#define ESCRITURA 1


int main (int argc, char **argv)
{
    /*
    _____________________
    Entrada por terminal
    ---------------------
    -i -o -n -c -b
    -i filename : archivo con el bombardeo
    -o filename : archivo de salida
    -n: cantidad de workers a generar.
    -c: tamaño del chunk, es decir, cantidad de líneas a leer por chunk.
    -b: bandera o flag que permite indicar si se quiere ver por consola la cantidad de expresiones regulares encontradas por cada proceso worker.
    */

    int in, n = 0, c = 0, b = 0;
    char * filename_in = NULL;
    char *filename_out = NULL;

    while ((in = getopt (argc, argv, "i:o:n:c:b")) != -1)
    switch (in)
    { 
        case 'n': //Numero de workers
            sscanf(optarg, "%d", &n);
            break;
        case 'c': //tamaño del chunks
            sscanf(optarg, "%d", &c);
            break;
        case 'b': //bandera
            sscanf(optarg, "%d", &c);
            break;
        case 'i': //Archivo entrada
            filename_in = optarg;
            break;
        case 'o': //Archivo de salida
            filename_out = optarg;
            break;
    }
    //Inicializamos para comunicarnos con los hijos 
    int pid;

    //Se forman las pipes
    int** pipes = (int**)malloc(sizeof(int*)*p); 

    //Definicion de pipes siendo esto una matriz que [i][j]
    for(int i = 0; i<p;i++)
    {
        pipes[i] = (int*)malloc(sizeof(int)*2);
    }
}