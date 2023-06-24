//-----LABORATORIO 2 -------//
//ARCHIVO PRINCIPAL PARA EL PROGRAMA
/*RAMO: SISTEMAS OPERATIVOS*/
/*AUTORES: Franco cornejo - Marcelo Godoy*/


#include <ctype.h>  //getopt
#include <stdio.h>  //entrada y salida
#include <stdlib.h> //memoria dinamica
#include <string.h> //manejo de string
#include <unistd.h> //Uso de fork(), pipes(), entre otros
#include <sys/wait.h> //Constantes simbólicas (waitpid(), wait(), etc)
#include <sys/types.h> //Uso de tipos de datos ( pid_t )
#include "funciones.h" //Funciones para volcar archivo en lista

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
    -w: cantidad de workers a generar.
    -c: tamaño del chunk, es decir, cantidad de líneas a leer por chunk.
    -b: bandera o flag que permite indicar si se quiere ver por consola la cantidad de expresiones regulares encontradas por cada proceso worker.
    */

    int in, w = 0, c = 0, b = 0;
    char *filename_in = NULL;
    char *filename_out = NULL;

    while ((in = getopt(argc, argv, "i:o:w:c:b")) != -1)
    {
        switch (in)
        {
            case 'w': //Cantidad de workers
                sscanf(optarg, "%d", &w);
                break;
            case 'c': //Tamaño del chunk
                sscanf(optarg, "%d", &c);
                break;
            case 'b': //Bandera
                b++;
                break;
            case 'i': //Archivo entrada
                filename_in = optarg;
                break;
            case 'o': //Archivo de salida
                filename_out = optarg;
                break;
        }
    }

    char parametros[200];
    sprintf(parametros, "%s %s %d %d %d", filename_in, filename_out, w, c, b);
    

    // Llamar al validador
    int parametrosValidos = validarParametros(parametros);
    if ( parametrosValidos){
        pid_t pid = fork(); // Crear el proceso hijo
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) { // Código para el proceso hijo
            
            char workers[10], chunks[10], flags[10];
            sprintf(workers, "%d", w);
            sprintf(chunks, "%d", c);
            sprintf(flags, "%d", b);
            execlp("./broker", "./broker", filename_in, filename_out, workers, chunks, flags, (char *)NULL);
            perror("execlp");
            // terminanos el proceso
            exit(EXIT_FAILURE);
        } else { // Código para el proceso padre
            wait(NULL); // Esperar a que el hijo termine
        }
    }else{
        
    }

    return 0;
}

