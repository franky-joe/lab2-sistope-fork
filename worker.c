#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "fworker.h"

#define MAX_BUFFER 1024

int main() {
    char buffer[MAX_BUFFER];
    int n;
    int is_finished = 0;
    int expresiones_resividas = 0;
    int expresiones_SI = 0;
    int expresiones_NO = 0;

    while (!is_finished && (n = read(STDIN_FILENO, buffer, MAX_BUFFER)) > 0) {
        buffer[n] = '\0';

      //if (strncmp(buffer, "FIN", 3) == 0) {
        if (buscar_FIN(buffer)) {
            printf("+Total Expresiones resividas por %d = %d\n", getpid(), expresiones_resividas);
            printf("+Expreciones SI por %d = %d\n",getpid() , expresiones_SI);
            printf("+Expreciones NO por %d = %d\n",getpid() , expresiones_NO);
            break;
        } else {
            expresiones_resividas ++;
            if(verificaER(buffer)){
                expresiones_SI ++;
                printf("Si-%d-%s\n", getpid(), buffer);
                fflush(stdout);
            }else{
                expresiones_NO ++;
                printf("NO-%d-%s\n", getpid(), buffer);
                fflush(stdout);

            }
        }
    }

    // Realizar cualquier limpieza o tareas adicionales antes de terminar

    return 0;
}

