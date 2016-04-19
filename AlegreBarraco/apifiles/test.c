#include "helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* FALTA VER:
 * Este codigo iría dentro de NuevoNimhe().
 * Falta revisar que despues de una linea comentario, no hayan mas comentarios.
 * Falta revisar solo haya una sola linea p y que sea entre c y e.
 * Falta revisar que haya nlados cantidad de lineas despues de p.
 */

int main () {
    char* line = NULL; // Línea leída.
    char* subline = NULL;
    char* ptr = NULL;
    int read = 0; // Cantidad de caracteres leídos.
    int argc = 0;
    bool flag = true; // True si seguimos leyendo, false de lo contrario.
    char* error_lectura = "Error en formato de entrada\n";
    u32 nvertices = 0, nlados = 0, lvertice = 0, rvertice = 0, nlados_tmp = 0;
    par_t par = NULL;

    while (flag) {
        line = readline_from_stdin();
        if (line == NULL) {
            flag = false;
            printf("%s", error_lectura);
        } else {
            read = strlen(line);
            if (read > 0) {
                if (line[0] == 'p') {
                    if ((par = handle_p_line(line)) != NULL) {
                        nvertices = get_l(par);
                        nlados = get_r(par);
                        nlados_tmp = nlados;
                        free(par);
                        par = NULL;
                    } else {
                        flag = false;
                        printf("%s", error_lectura);
                    }
                    printf("Grafo tiene -%u- vertices.\n", nvertices);
                    printf("Grafo tiene -%u- lados.\n", nlados);
                } else if (line[0] == 'e') {
                    if ((par = handle_e_line(line)) != NULL) {
                        lvertice = get_l(par);
                        rvertice = get_r(par);
                        free(par);
                        par = NULL;
                    } else {
                        flag = false;
                        printf("%s", error_lectura);
                    }
                    printf("Vertice izquiero: %u. Vertice derecho: %u.\n", lvertice, rvertice);
                } else if (line[0] != 'c' || line[1] != ' ') {
                    flag = false;
                    printf("%s", error_lectura);
                }
            } else if (read == 0) {
                flag = false;
            }
            free(line);
            line = NULL;
        }
    }

    return 0;
}
