#include <stdio.h>
#include <string.h>
//#include <regex.h>
#include <unistd.h>

#include "funciones.h"

/* Para usar este programa ./a.out -i prueba_1000.txt -o salida.txt */


int main(int argc, char *argv[]) {
    int opt;
    char *nombre_archivo_entrada = NULL;
    char *nombre_archivo_salida = NULL;
    int chunks = 0;
    int workers = 0;
    int mostrar_datos = 0;
    
    // gestion de parametros de entrada
    while((opt = getopt(argc, argv, "i:o:bc:n:")) != -1) {
        switch(opt){
            case 'i':
                nombre_archivo_entrada = optarg;
                break;
            case 'o':
                nombre_archivo_salida = optarg;
                break;
            case 'b':
                mostrar_datos ++;
                break;
            case 'c':
                chunks = atoi(optarg);
                break;
            case 'n':
                workers = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Uso: %s [-i inputfile] [-o outputfile] [-b] [-c chunks] [-n workers]\n", argv[0]);
                return 1;
        }
    }
}
    
    int cantidad_lineas_archivo = contadorLineas(nombre_archivo_entrada);
    char * lista_string[cantidad_lineas_archivo + 3];
    volcarArchivo(nombre_archivo_entrada, lista_string, cantidad_lineas_archivo);

    int total_expre_si = 0;
    int total_expre_no = 0;


    /* --------Aqui codigo verificacion-----------------*/

    return 0;
}

