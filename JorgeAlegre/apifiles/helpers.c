#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "helpers.h"

struct _par_t {
    u32 l;
    u32 r;
};

char* strstrip(char* s) {
    size_t size;
    char *end;

    size = strlen(s);

    if (!size)
        return s;

    end = s + size - 1;
    while (end >= s && isspace(*end))
        end--;
    *(end + 1) = '\0';

    while (*s && isspace(*s))
        s++;

    return s;
}

char* readline_from_stdin() {
    char* line = NULL; // String con lectura de stdin.

    // Línea tendrá MAX_LINE_LENGTH caracteres más '\n'.
    line = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
    assert(line != NULL); // Asegurar que se pudo pedir memoria.

    // Leer de stdin.
    if (fgets(line, MAX_LINE_LENGTH + 1, stdin) != NULL) {
        if (line[strlen(line) - 1] != '\n') {
            // Línea no cumple con formato. Libero memoria y devuelvo NULL.
            free(line);
            line = NULL;
        }
    }

    return (strstrip(line));
}

char* strcopy(const char* string) {
    size_t len = strlen(string) + 1;
    void* new = malloc(len * sizeof(char));
    assert(new != NULL);

    return (char*)memcpy(new, string, len);
}

par_t handle_p_line(char* line) {
    assert(line != NULL);
    char* liberar = NULL;
    char* copy = NULL; // Copia de "line" para usar strtok().
    char* subline = NULL; // Usada para guardar cada substring de la línea.
    int i = 0; /* Índice del substring de "line". Si llega a 4, línea ...
    no tiene formato requerido. */
    unsigned int char_i = 0; // Índice de los caracteres en "subline".
    bool es_num = true; // Si true, subline es un número.
    par_t par = NULL; // n vértices y m lados serán guardados en par.

    par = calloc(1, sizeof(struct _par_t));
    assert(par != NULL);

    copy = strcopy(line);
    liberar = copy;

    /* Si en algún momento, uno de los 4 substrings de la línea no respeta su
     * formato requerido, par se libera y se la setea a NULL para que deje de
     * buscar.
     * i empieza en 0. Por cada substring, se aumenta en 1. Si llega a 4
     * y strsep no devuelve NULL, habían más cosas despues del cuarto substring.
     */
    while (par != NULL && i < 5 && (subline = strtok(copy, " ")) != NULL) {
        switch (i) {
            case 0:
                copy = NULL;
                // "subline" debe ser igual a "p".
                // strcmp devuelve 0  si strings son iguales.
                if (strcmp(subline, "p") != 0) {
                    free(par);
                    par = NULL;
                }
                break;
            case 1:
                // "subline" debe ser igual a "edge".
                if (strcmp(subline, "edge") != 0) {
                    free(par);
                    par = NULL;
                }
                break;
            case 2:
                // "subline" será un número.
                // Revisar que solo sea un número.
                char_i = 0;
                while (es_num && char_i < strlen(subline)) {
                    es_num = es_num && isdigit(subline[char_i]);
                    char_i++;
                }
                if (es_num) {
                    par->l = strtoul(subline, NULL, 10);
                } else {
                    free(par);
                    par = NULL;
                }
                break;
            case 3:
                // "subline" será un número.
                // Revisar que solo sea un número.
                char_i = 0;
                while (es_num && char_i < strlen(subline)) {
                    es_num = es_num && isdigit(subline[char_i]);
                    char_i++;
                }
                if (es_num) {
                    par->r = strtoul(subline, NULL, 10);
                } else {
                    free(par);
                    par = NULL;
                }
                break;
            default:
                // i == 4, mal formato.
                free(par);
                par = NULL;
        }
        i++;
    }

    if (liberar != NULL) {
        free(liberar);
        liberar = NULL;
    }

    return (par);
}

par_t handle_e_line(char* line) {
    assert(line != NULL);
    char* liberar = NULL;
    char* copy = NULL;
    char* subline = NULL; // Usada para guardar cada substring de la línea.
    int i = 0; /* Índice del substring de "line". Si llega a 3, línea ...
    no tiene formato requerido. */
    unsigned int char_i = 0; // Índice de los caracteres en "subline".
    bool es_num = true; // Si true, subline es un número.
    par_t par = NULL; // n vértices y m lados serán guardados en par.

    par = calloc(1, sizeof(struct _par_t));
    assert(par != NULL);

    copy = strcopy(line);
    liberar = copy;

    /* Si en algún momento, uno de los 3 substrings de la línea no respeta su
     * formato requerido, par se libera y se la setea a NULL para que deje de
     * buscar.
     * i empieza en 0. Por cada substring, se aumenta en 1. Si llega a 3
     * y strsep no devuelve NULL, habían más cosas despues del tercer substring.
     */
    while (par != NULL && i < 4 && (subline = strtok(copy, " ")) != NULL) {
        switch (i) {
            case 0:
                copy = NULL;
                // "subline" debe ser igual a "e".
                // strcmp devuelve 0 si strings son iguales.
                if (strcmp(subline, "e") != 0) {
                    free(par);
                    par = NULL;
                }
                break;
            case 1:
                // "subline" será un número.
                // Revisar que solo sea un número.
                char_i = 0;
                while (es_num && char_i < strlen(subline)) {
                    es_num = es_num && isdigit(subline[char_i]);
                    char_i++;
                }
                if (es_num) {
                    par->l = strtoul(subline, NULL, 10);
                } else {
                    free(par);
                    par = NULL;
                }
                break;
            case 2:
                // "subline" será un número.
                // Revisar que solo sea un número.
                char_i = 0;
                while (es_num && char_i < strlen(subline)) {
                    es_num = es_num && isdigit(subline[char_i]);
                    char_i++;
                }
                if (es_num) {
                    par->r = strtoul(subline, NULL, 10);
                } else {
                    free(par);
                    par = NULL;
                }
                break;
            default:
                // i == 3, mal formato.
                free(par);
                par = NULL;
        }
        i++;
    }

    if (liberar != NULL) {
        free(liberar);
        liberar = NULL;
    }

    return (par);
}

u32 get_l(par_t par) {
    assert(par != NULL);
    return (par->l);
}

u32 get_r(par_t par) {
    assert(par != NULL);
    return (par->r);
}
