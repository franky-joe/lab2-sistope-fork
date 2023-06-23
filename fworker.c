#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fworker.h"

int verificaER(char pruebas[]){
    /*
    Funcion:  analiza el texto y verifica si cumple con la exprecion regular dada
    Rec: Una lista de caracteres
    Dom: un 1 o un 0

    Analizador lexico 
    con las columnas siendo A-C-G-T 
    con las filas representando los estados 
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

int buscar_FIN(const char* cadena) {
    /*
        fucion: busca la cadena "FIN"
        Rec: Una lista de caracteres
        Dom: 1 o 0
    */
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == 'F' && cadena[i+1] == 'I' && cadena[i+2] == 'N') {
            return 1; // Se encontró "FIN" en la cadena
        }
        i++;
    }
    return 0; // No se encontró "FIN" en la cadena
}
