#include <stdio.h>
#include <string.h>
//#include <regex.h>
#include <unistd.h>
#include "funciones.h"

int verificaER(char pruebas[]){
    /*
    Funcion:  analiza el texto y verifica si cumple con la exprecion regular dada
    Rec: Una lista de caracteres
    Dom: un 1 o un 0

    Analizador lexico 
    con las columnas siendo A-C-G-T 
    con las filas representando los estados 
    */
    /*
    int matriz[4][4] =  {{0,0,1,0}, 
                        {0,0,0,2}, 
                        {0,3,0,2}, 
                        {-1,-1,-1,-1}};
    */
    int matriz[4][4] =  {{0,0,1,0}, 
                        {0,0,1,2}, 
                        {0,3,1,2}, 
                        {-1,-1,-1,-1}};
    //indicara la columna
    int ptr1 = 0;
    //indicara la fila
    int ptr2 = 0;
    //empezamos a probar
    for (int i = 0; i < strlen(pruebas) ; i++)
    {
        //Transformo la letra a un numero que la representa en la matriz 
        if (pruebas[i] == 'A'){
            ptr1 = 0;
        }else if (pruebas[i] == 'C'){
            ptr1 = 1;
        }else if (pruebas[i] == 'G'){
            ptr1 = 2;
        }else if (pruebas[i] == 'T'){
            ptr1 = 3;
        }
        //final del automata
        if (ptr2 == -1){
            return 1;}
        //Defino hacia donde se mueve el automata
        int Auxptr2 = matriz[ptr2][ptr1];
        ptr2 = Auxptr2; }  
    return 0;
}

int contadorLineas(char *filename) {
    /*
        Funcion: resive un string con el nombre de un archivo y retorna la cantidad de lineas del archivo
        Rec: una String con el nombre del archivo 
        Dom: un entero
    */
    int count = 0;
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error al abrir el archivo %s\n", filename);
        return -1;
    }

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}

int volcarArchivo(char *filename, char* lista_string[], int numero_lineas_archivo){
    /*
    Funcion:  Resive un string con el nombre del archivo y una lista de string donde volcara el contenido del archivo, linea a linea 
    Rec: Dos string, uno con el nombre del archio, Un puntero a una lista de strings y el numero de lineas del archio
    Doc: un entero que indica que todo salio correcto
    */
    FILE *fp = fopen(filename, "r");
    char  buffer[100];
    int count = 0;
    while( count < numero_lineas_archivo){
        fgets(buffer, 100, fp);
        lista_string[count] = strdup(buffer);
        count ++;
    }
    //while (lista_string[count] != NULL){
    fclose(fp);
    return 1;
}

int volcarListaEnArchivo(char *filename, char* lista_string[], int largo_lista){
    /*
    Funcion: vuelaca el contenido de una lista en un archivo 
    Rec: Un String con el nombre del archiovo, un puntero a una lista de strings y un entero con el largo de la lista
    Doc: un entero que indica que todo salio correcto
    */
    FILE *fp = fopen(filename, "w");
    int count = 0;
    while (count < largo_lista){
        fprintf(fp, "%s", lista_string[count]);
        count ++;
    }
        
    return 1;
}

int validarParametros(const char *parametros)
{
    char filename_in[100];
    char filename_out[100];
    int w, c, b;

    // Descomponer la cadena de parámetros
    sscanf(parametros, "%s %s %d %d %d", filename_in, filename_out, &w, &c, &b);

    // Verificar que los valores sean válidos
    if (w < 1 || c < 1 ) {
        printf("Error: valores de parámetros inválidos\n");
        return 0;
    }
    // Verificar si el archivo de entrada existe
    if (access(filename_in, F_OK) == -1) {
        printf("Error: el archivo de entrada no existe\n");
        return 0;
    }
    // Verivicar que la cantidad de lineas que se solicita analizar sea menor que las del archivo
    int cantidad_lineas_archivo = contadorLineas(filename_in);
    int lineas_que_se_solicita_leer =  c * w;
    if (cantidad_lineas_archivo < lineas_que_se_solicita_leer){
       printf("Error: chunks * workers es un mayor a las Expreiones que se encuentran en %s\n", filename_in);
       return 0;

    } 

    return 1;
}
