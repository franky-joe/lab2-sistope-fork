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
    int mostrar_datos = 0;
    
    // gestion de parametros de entrada
    while((opt = getopt(argc, argv, "i:o:b")) != -1) {
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
            default:
                break;
        }
    }
    
    int cantidad_lineas_archivo = contadorLineas(nombre_archivo_entrada);
    char * lista_string[cantidad_lineas_archivo + 3];


    volcarArchivo(nombre_archivo_entrada, lista_string, cantidad_lineas_archivo);
    int total_expre_si = 0;
    int total_expre_no = 0;

    int i = 0;
    while ( i < cantidad_lineas_archivo){
        char buffer[100] = "Si " ;
        char buffer2[100] = "No " ;

        if (verificaER(lista_string[i])){
            total_expre_si ++;
            strcat(buffer, lista_string[i]);
            //printf( "%s", buffer );
            lista_string[i] = strdup(buffer);
            
        }else { 
            total_expre_no ++;
            strcat(buffer2, lista_string[i]);
            //printf( "%s", buffer2 );
            lista_string[i] = strdup(buffer2);
        }
        i++;
    }
    if (mostrar_datos){ 
        char total_si[100] = "Total de expresiones que SI son regulares: "  ;
        char total_no[100] = "Total de expresiones que NO son regulares: "  ;
        char total_leidas[100] = "Total lineas leidas: ";
        char buffer[100];

        sprintf(buffer, "%d", total_expre_si);
        strcat(total_si, buffer);
        strcat(total_si, "\n");
        lista_string[i] = total_si;
            
        buffer[0] = '\0';
        sprintf(buffer, "%d", total_expre_no);
        strcat(total_no, buffer);
        strcat(total_no, "\n");
        lista_string[i + 1] = total_no;
        
        buffer[0] = '\0';
        sprintf(buffer, "%d", cantidad_lineas_archivo);
        strcat(total_leidas, buffer);
        strcat(total_leidas, "\n");
        lista_string[ i + 2] = total_leidas ;  

        volcarListaEnArchivo(nombre_archivo_salida, lista_string, cantidad_lineas_archivo+3);
        /*
        buffer = "";
        sprintf(buffer, ) 
        */
        /*
        strcat(total_no, (char)total_expre_no);
        strcat(total_leidas, (char)cantidad_lineas_archivo);

        lista_string[i+1] = strdup(total_no);
        lista_string[i+2] = strdup((char)cantidad_lineas_archivo);
        */
    }else {
        volcarListaEnArchivo(nombre_archivo_salida, lista_string, cantidad_lineas_archivo);
    }


    /* --------Aqui codigo verificacion-----------------*/

    return 0;
}

