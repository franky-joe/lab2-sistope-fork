#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "fbroker.h"

#define MAX_BUFFER 100000

int insertarEnArchivo(char* string, char* NombreArchivo) {
    FILE* archivo = fopen(NombreArchivo, "a"); // Abrir el archivo en modo "append"
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }

    int resultado = fputs(string, archivo); // Insertar la cadena en el archivo
    if (resultado == EOF) {
        perror("Error al escribir en el archivo");
        fclose(archivo);
        return -1;
    }

    fclose(archivo);
    return 0;
}

int borrarLineasConMas(char* archivoEntrada, char* archivoSalida, char* prefijo) {
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
    char buffer[1024];
    int contador = 0, no = 0;

    FILE* archivo = fopen(NombreArchivo, "r+"); 
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }
    fseek(archivo, 0, SEEK_SET);
    while (fgets(buffer, sizeof(buffer), archivo)) {
        printf("%s", buffer);
        if (strncmp(buffer, "Si", 2) == 0) {
            contador++;
        }else{
            no++;
        }
    }
    fprintf(archivo, "\nTotal de ocurrencias de 'Sí': %d\n", contador);
    fprintf(archivo, "\nTotal de ocurrencias de 'No': %d\n", no);
    fclose(archivo);
    return contador;
}


int main(int argc, char *argv[]) {
    
    //sprintf(parametros, "%s %s %d %d %d", filename_in, filename_out, w, c, b);
    char* filename = argv[1];
    char* filename_out = argv[2];
    int worker = atoi(argv[3]);
    int chunk = atoi(argv[4]);
    int b = atoi(argv[5]);
    int i;

    int pipefds[worker][2][2]; // Matriz de tuberías para la comunicación entre broker y workers.

    for(i = 0; i < worker; i++) {
        if (pipe(pipefds[i][0]) == -1 || pipe(pipefds[i][1]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t cpid = fork();

        if (cpid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (cpid == 0) {
            close(pipefds[i][0][1]); // Cerramos el extremo de escritura del primer pipe en el hijo.
            close(pipefds[i][1][0]); // Cerramos el extremo de lectura del segundo pipe en el hijo.

            dup2(pipefds[i][0][0], STDIN_FILENO);
            dup2(pipefds[i][1][1], STDOUT_FILENO);

            execlp("./worker", "./worker", (char *) NULL);
            exit(EXIT_SUCCESS);
        } else {
            close(pipefds[i][0][0]); // Cerramos el extremo de lectura del primer pipe en el padre.
            close(pipefds[i][1][1]); // Cerramos el extremo de escritura del segundo pipe en el padre.
        }
    }

    int cantidad_lineas_archivo = contadorLineas(filename);
    char * lista_string[cantidad_lineas_archivo];
    volcarArchivo(filename, lista_string, cantidad_lineas_archivo);
    /*
    int j = 0;
    for (j = 0; j < cantidad_lineas_archivo; j++) {
        printf("%d %s\n", j, lista_string[j]);
    }
    j=0;
    */


    int worker_index = 0;
    int expresiones_por_worker = cantidad_lineas_archivo / worker;
    int expresiones_restantes = cantidad_lineas_archivo % worker;
    int expresiones_totales_para_enviar = chunk * worker;
    char line[MAX_BUFFER];
    int j=0;

    // Alternamos el paso de los datos a los workers
    // printf("Lineas totales: %d\n", expresiones_totales_para_enviar);
    int lineas_enviadas = 0; // Declaración de la variable lineas_enviadas
    
    while (j < expresiones_totales_para_enviar) {
        if (lineas_enviadas < cantidad_lineas_archivo) {
            usleep(1100);
            memset(line, 0, sizeof(line)); // Limpiar el buffer antes de escribir en el pipe
            strncpy(line, lista_string[j], sizeof(line) - 1); // Copiar la línea al buffer
            write(pipefds[worker_index][0][1], line, strlen(line));
            lineas_enviadas++; // Incrementar lineas_enviadas después de enviar una línea
            // pequeño tiempo espera
            //waitpid(cpid, NULL, 0);
        }
        
        worker_index++;
        if (worker_index == worker) {
            worker_index = 0;
        }
        j++;
    }


    
    for (i = 0; i < worker; i++) {
        //write(pipefds[i][0][1], "FIN", 4); // Enviando señal de finalización a cada worker.
        usleep(1500);
        write(pipefds[i][0][1], "FIN", 3); // Enviando señal de finalización a cada worker.

        close(pipefds[i][0][1]); // Cerramos el extremo de escritura del primer pipe en el padre después de enviar datos.
    }
    //char *expresiones_revisadas[expresiones];
    char* NombreArchivo = "Corregir.txt";
    char* NombreArchivoCorregido = "Corregido.txt";
    char* borrar = "+";
    // aqui quiero que leea hasta un salto de linea 
    if (b == 1){
        for (i = 0; i < worker; i++) {
            while ((read(pipefds[i][1][0], line, MAX_BUFFER)) > 0) { // Leer la respuesta de cada worker.
                printf("%s", line); // Imprimir la respuesta del worker.
                insertarEnArchivo(line, NombreArchivo);
            }
            close(pipefds[i][1][0]); // Cerramos el extremo de lectura del segundo pipe después de leer datos.
            wait(NULL);
        }
        // Aqui falta que se escriba el archivo de salida
        borrarLineasConMas(NombreArchivo, NombreArchivoCorregido, borrar);
        contador(NombreArchivoCorregido);
    }else{
        // Solo escribir el archivo de salida 

    }
    // matamos al broker
    exit(EXIT_SUCCESS);

    return 0;
}

