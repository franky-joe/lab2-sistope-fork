#include <stdio.h>
#include <string.h>
//#include <regex.h>
#include <unistd.h>
#include "fbroker.h"
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
        buffer[strcspn(buffer, "\n")] = '\0';
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

