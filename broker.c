#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "fbroker.h"

#define MAX_BUFFER 100000

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

    // aqui quiero que leea hasta un salto de linea 
    if (b == 1){
        for (i = 0; i < worker; i++) {
            while ((read(pipefds[i][1][0], line, MAX_BUFFER)) > 0) { // Leer la respuesta de cada worker.
                printf("%s", line); // Imprimir la respuesta del worker.
            }
            close(pipefds[i][1][0]); // Cerramos el extremo de lectura del segundo pipe después de leer datos.
            wait(NULL);
        }
        // Aqui falta que se escriba el archivo de salida
    }else{
        // Solo escribir el archivo de salida 

    }
    // matamos al broker
    exit(EXIT_SUCCESS);

    return 0;
}

