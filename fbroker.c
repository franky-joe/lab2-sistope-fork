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

int insertarEnArchivo(char* string, char* NombreArchivo) {
    /*
    Funcion: vuelca todos los pipes en un unico archivo
    Rec: Un String pipe, un string con el nombre del archivo a volcar
    Doc: Un archivo
    */
    FILE* archivo = fopen(NombreArchivo, "a"); // Abrir el archivo en modo "append"
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    int resultado = fputs(string, archivo); // Insertar la cadena en el archivo
    fclose(archivo);
    if (resultado == EOF) {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return -1;
    }
    return 0;
}

int correccionFormato(char* archivoEntrada, char* archivoSalida, char* prefijo) {
    /*
    Funcion: Modifica el archivo eliminando los elementos basura
    Rec:Un archivo que se busca arreglar, un archivo que guardara las lineas arregladas y el elemento a borrar
    Doc: Un archivo
    */
    char buffer[1024];

    FILE* archivoE = fopen(archivoEntrada, "r"); // Abrir el archivo en modo "append"
    if (archivoE == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    FILE* archivoS = fopen(archivoSalida, "w+"); // Abrir el archivo en modo "append"
    if (archivoS == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), archivoE)) {
        if (strncmp(buffer, prefijo, strlen(prefijo)) != 0) {
            char* inicio = strchr(buffer, '-');
            char* fin = NULL;

            while (inicio != NULL) {
                fin = strchr(inicio + 1, '-');

                if (fin != NULL) {
                    memmove(inicio+1, fin + 1, strlen(fin));
                } else {
                    memmove(inicio+1, inicio + 1 + strlen(inicio + 1), strlen(inicio + 1));
                }

                inicio = strchr(inicio + 1, '-');
            }
            fputs(buffer, archivoS);
        }
    }
    
    fclose(archivoS);
    fclose(archivoE);
    return 1;
    }

int contador(char* NombreArchivo) {
    /*
    Funcion: Hace un conteo de Si y NO dentro del archivo junto con su conteo final
    Rec: Un archivo
    Doc: Un archivo
    */
    char buffer[1024];
    int contador = 0, no = 0;

    FILE* archivo = fopen(NombreArchivo, "r+"); 
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }
    fseek(archivo, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), archivo)) {
        if (strncmp(buffer, "Si", 2) == 0) {
            contador++;
        }else{
            no++;
        }
    }
    fprintf(archivo, "Total de ocurrencias de 'Sí': %d\n", contador);
    fprintf(archivo, "Total de ocurrencias de 'No': %d\n", no);
    fprintf(archivo, "Total de expresiones revisadas: %d\n", no + contador);
    fclose(archivo);
    return contador;
}

int salida(char* archivoEntrada, char* archivoSalida) {
    /*
    Funcion: Termina de corregir los errores de formato
    Rec: Dos archivos, la salida oficial y el archivo a corregir
    Doc: Un archivo
    */
    char linea[100] ;
    FILE* archivoE = fopen(archivoEntrada, "r"); // Abrir el archivo en modo "append"
    if (archivoE == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    FILE* archivoS = fopen(archivoSalida, "w+"); // Abrir el archivo en modo "append"
    if (archivoS == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }
    while (fgets(linea, sizeof(linea), archivoE)) {
        // Eliminar el salto de línea al final de la línea
        
        // Buscar la posición del guion en la línea
        char* guion = strchr(linea, '-');
        if (guion != NULL) {
            int longitud = strcspn(linea, "\n");
            linea[longitud-1] = ' ';
            longitud = strcspn(linea, "\n");
            linea[longitud] = '\0';
            // Reorganizar la línea y escribirla en el archivo de salida
            *guion = '\0';
            fprintf(archivoS, "%s%s\n", guion +1, linea);
        }else{
            fputs(linea, archivoS);
        }
    }
    
    fclose(archivoE);
    fclose(archivoS);
    return 1;
}
