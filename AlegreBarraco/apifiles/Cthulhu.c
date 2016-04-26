#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "Cthulhu.h"

struct _par_t {
    u32 l;
    u32 r;
};

struct _node_t {
    u32 nombre;
    u32 index;
    list_t next;
};

struct _vertex_t{
    u32 nombre;
    u32 grado;
    u32 color;
};

struct NimheSt{
    u32 nvertices; // Número de vertices en el grafo.
    u32 nlados; // Número de lados en el grafo.
    u32 ncolores; // Número de colores usados hasta el momento para el coloreo propio.
    u32* orden_actual; // String del orden del grafo.
    VerticeSt* vertices; // Arreglo de vértices en orden original.
    list_t* vecinos; // Arreglo de listas de vecinos de vértices.
};

u32 ColorDelVertice(VerticeSt x) {
    return (x.color);
}

u32 GradoDelVertice(VerticeSt x) {
    return (x.grado);
}

u32 NombreDelVertice(VerticeSt x) {
    return (x.nombre);
}

void ImprimirVecinosDelVertice(VerticeSt x, NimheP G) {
    u32 i = 0; // Iterador de vecinos.
    u32 posv = 0; // Para buscar la posición de "x".
    bool encontrado = false; // Indica que encontré a "x" en "G".

    if (G != NULL) {
        while (posv < G->nvertices && !encontrado) {
            if ((G->vertices[posv]).nombre == x.nombre) {
                encontrado = true;
            } else {
                posv++;
            }
        }

        // Imprimir con comma todos menos el último vecino.
        while (i < (G->vertices[posv]).grado - 1) {
            // Imprime el nombre del vertice en la posicion de cada uno
            // de los vecinos del vertice de entrada.
            printf("%u,", G->vertices[(list_index(G->vecinos[posv], i))].nombre);
            i++;
        }

        // Imprimir con punto al último vecino.
        printf("%u.", G->vertices[(list_index(G->vecinos[posv], i))].nombre);
    }
}

NimheP NuevoNimhe() {
    char* line = NULL; // Línea leída.
    int read = 0; // Cantidad de caracteres leídos.
    bool flag = true; // True si seguimos leyendo, false de lo contrario.
    bool existel = false; // True si vértice ya fue procesado.
    bool exister = false; // True si vértice ya fue procesado.
    char* error_lectura = "Error en formato de entrada\n"; // Mensaje de error.
    u32 lvertice = 0; // Valor temp. del vértice izquierdo (v en "e v w").
    u32 rvertice = 0; // Valor temp. del vértice derecho (w en "e v w").
    u32 nlados_tmp = 1; /* Para saber cuántas veces leer lados de STDIN.
        Empieza con 1 para que el ciclo funcione. */
    u32 nlados_leidos = 0; // Para saber cuántos lados llevo leídos.
    u32 pos_rv = 0; // Indica la posición del vértice derecho leído.
    u32 pos_lv = 0; // Indica la posición del vértice izquierdo leído.
    par_t par = NULL; // Valores obtenidos de la lectura de cada línea.
    NimheP result = NULL; // Grafo que será devuelto por función.
    list_t vertices_cargados = list_empty(); // Para saber si ya leí un vértice.
    u32 pos_vertice_nuevo = 0; // Para saber hasta donde insertar vértice nuevo.

    while (flag && nlados_leidos < nlados_tmp) {
        line = readline_from_stdin();
        if (line == NULL) {
            flag = false;
            printf("%s", error_lectura);
            if (!DestruirNimhe(result)) {
                printf("Mala destrucción del grafo.\n");
            }
        } else {
            read = strlen(line);
            if (read > 0) {
                if (line[0] == 'p' && result == NULL) {
                    // Si grafo ya está inicializado, ya leí línea con "p".
                    if ((par = handle_p_line(line)) != NULL) {
                        result = calloc(1, sizeof(struct NimheSt));

                        // Obtengo datos leídos de línea.
                        result->nvertices = get_l(par);
                        result->nlados = get_r(par);
                        nlados_tmp = get_r(par);
                        result->ncolores = 0;

                        // Ya terminé de usar datos de línea leída
                        free(par);
                        par = NULL;

                        // Inicializar arreglo de vértices y vecinos.
                        result->vertices = calloc(result->nvertices, sizeof(struct _vertex_t));
                        result->vecinos = calloc(result->nvertices, sizeof(struct _node_t));
                    } else {
                        flag = false;
                        printf("%s", error_lectura);
                        if (!DestruirNimhe(result)) {
                            printf("Mala destrucción del grafo.\n");
                        }
                    }
                } else if (line[0] == 'e'  && result != NULL) {
                    if ((par = handle_e_line(line)) != NULL) {
                        lvertice = get_l(par);
                        rvertice = get_r(par);
                        printf("izquierdo: %u - derecho %u.\n", lvertice, rvertice);
                        free(par);
                        par = NULL;

                        existel = list_exists(vertices_cargados, lvertice);
                        exister = list_exists(vertices_cargados, rvertice);

                        if (!existel && !exister) {
                            // Ambos vértices son nuevos.
                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo + 1, rvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, lvertice);

                            pos_vertice_nuevo++;

                            result->vertices[pos_vertice_nuevo].nombre = rvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo - 1, lvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, rvertice);

                            pos_vertice_nuevo++;
                        } else if (!existel && exister) {
                            // Solo cargué derecho
                            pos_rv = list_search(vertices_cargados, rvertice);

                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_rv, rvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, lvertice);

                            // Agregar izquierdo como vecino de derecho.
                            result->vertices[pos_rv].grado++;
                            result->vecinos[pos_rv] = list_append(result->vecinos[pos_rv], pos_vertice_nuevo, lvertice);

                            pos_vertice_nuevo++;
                        } else if (existel && !exister) {
                            // Solo cargué izquierdo
                            pos_lv = list_search(vertices_cargados, lvertice);

                            result->vertices[pos_vertice_nuevo].nombre = rvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_lv, lvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, rvertice);

                            // Agregar derecho como vecino de izquierdo.
                            result->vertices[pos_lv].grado++;
                            result->vecinos[pos_lv] = list_append(result->vecinos[pos_lv], pos_vertice_nuevo, rvertice);

                            pos_vertice_nuevo++;
                        } else {
                            // Ambos vértices ya existen. Solo setear vecinos.
                            pos_lv = list_search(vertices_cargados, lvertice);
                            pos_rv = list_search(vertices_cargados, rvertice);
                            result->vertices[pos_lv].grado++;
                            result->vertices[pos_rv].grado++;
                            result->vecinos[pos_lv] = list_append(result->vecinos[pos_lv], pos_rv, rvertice);
                            result->vecinos[pos_rv] = list_append(result->vecinos[pos_rv], pos_lv, lvertice);
                        }
                        nlados_leidos++;
                    } else {
                        flag = false;
                        printf("%s", error_lectura);
                        if (!DestruirNimhe(result)) {
                            printf("Mala destrucción del grafo.\n");
                        }
                    }
                } else if ((line[0] == 'c' && line[1] == ' ') && result == NULL) {
                    // Comentario. No hacer nada.
                } else {
                    flag = false;
                    printf("%s", error_lectura);
                    if (!DestruirNimhe(result)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                }
            } else if (read == 0) {
                flag = false;
            }
            free(line);
            line = NULL;
        }
    }
    vertices_cargados = list_destroy(vertices_cargados);

    return (result);
}

int DestruirNimhe(NimheP G) {
    int result = 1;
    u32 i = 0; // Iterador para liberar listas de vecinos.
    u32 j = 0; // Iterador para

    if (G != NULL) {
        if (G->vecinos != NULL) {
            for (i = 0; i < G->nvertices; i++) {
                G->vecinos[i] = list_destroy(G->vecinos[i]);
                if (G->vecinos[i] != NULL) result = 0;
            }
            free(G->vecinos);
            G->vecinos = NULL;
        }
        if (G->vertices != NULL) {
            free(G->vertices);
            G->vertices = NULL;
        }
        if (G->orden_actual != NULL) {
            free(G->orden_actual);
            G->orden_actual = NULL;
        }
        free(G);
        G = NULL;
    }

    return (result);
}

u32 NumeroDeVertices(NimheP G) {
    u32 result = 0;

    if (G != NULL) {
        result = G->nvertices;
    }

    return (result);
}

u32 NumeroDeLados(NimheP G) {
    u32 result = 0;

    if (G != NULL) {
        result = G->nlados;
    }

    return (result);
}

u32 NumeroVerticesDeColor(NimheP G, u32 i) {
    u32 result = 0;

    if (G != NULL) {
        for (u32 j = 0; j < G->nvertices; j++) {
            if (G->vertices[j].color == i) result++;
        }
    }

    return (result);
}

u32 ImprimirVerticesDeColor(NimheP G, u32 i) {
    u32 result = 0;

    if (G != NULL) {
        for (u32 j = 0; j < G->nvertices; j++) {
            if (G->vertices[j].color == i) {
                if (result == 0) {
                    printf("Vertices de Color %u: %u", i, G->vertices[j].nombre);
                } else {
                    printf(",%u", G->vertices[j].nombre);
                }
                result++;
            }
        }
    }

    if (result == 0) {
        printf("No hay vertices de color %u\n", i);
    } else {
        printf(".\n");
    }

    return (result);
}

u32 CantidadDeColores(NimheP G) {
    u32 result = 0;

    if (G != NULL) {
        result = G->ncolores;
    }

    return (result);
}

VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i){
    VerticeSt result;

    return (result);
}

VerticeSt IesimoVecino(NimheP G, VerticeSt x, u32 i) {
    VerticeSt result;

    return (result);
}

int Chidos(NimheP G) {
    int result = 0;

    

    return (result);
}

u32 Greedy(NimheP G);

void OrdenNatural(NimheP G);

void OrdenWelshPowell(NimheP G);

void ReordenAleatorioRestringido(NimheP G);

void GrandeChico(NimheP G);

void ChicoGrande(NimheP G);

void Revierte(NimheP G);

void OrdenEspecifico(NimheP G,u32* x);

list_t list_empty() {
    list_t list = NULL;

    return (list);
}

u32 list_length(list_t list) {
    u32 length = 0;
    list_t aux = list;

    if (aux != NULL) {
        while (aux != NULL && aux->next != NULL) {
            aux = aux->next;
            length++;
        }
        length++;
    }

    return (length);
}

list_t list_destroy(list_t list) {
    list_t aux = NULL;

    if (list != NULL) {
        //Destroy every node but first.
        while (list->next != NULL) {
            //While the second node exists.
            //Make aux point to first node.
            aux = list;
            //Make list point to second node.
            list = aux->next;
            //Clear first node.
            aux->next = NULL;
            free(aux);
            aux = NULL;
        }

        //Destroy first node.
        free(list);
        list = NULL;
    }

    assert(list == NULL);

    return (list);
}

u32 list_search(list_t list, u32 nombre) {
    list_t aux = list;
    u32 result = 0;
    bool encontrado = false;

    while (aux != NULL && !encontrado) {
        if (nombre == aux->nombre) {
            result = aux->index;
            encontrado = true;
        } else {
            aux = aux->next;
        }
    }

    return (result);
}

u32 list_exists(list_t list, u32 nombre) {
    list_t aux = list;
    bool result = false;

    while (aux != NULL && !result) {
        if (nombre == aux->nombre) {
            result = true;
        } else {
            aux = aux->next;
        }
    }

    return (result);
}

u32 list_index(list_t list, u32 i) {
    list_t aux = list;
    u32 j = 0;

    while (j < i) {
        aux = aux->next;
        j++;
    }

    return (aux->index);
}

list_t list_append(list_t list, u32 index, u32 nombre) {
    list_t aux = NULL;

    if (list == NULL) {
        list = calloc(1, sizeof(struct _node_t));
        assert(list != NULL);
        list->index = index;
        list->nombre = nombre;
        list->next = NULL;
    } else {
        aux = list;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = calloc(1, sizeof(struct _node_t));
        aux->next->next = NULL;
        aux->next->index = index;
        aux->next->nombre = nombre;
    }

    return (list);
}

list_t list_remove(list_t list, u32 nombre) {
    unsigned int length = list_length(list);
    unsigned int n = 1, i = 1;
    list_t aux = list;
    list_t aux1 = list;

    if (list != NULL) {
        while (n < length && nombre != aux->nombre) {
            aux = aux->next;
            n++;
        }
        if (n == length && nombre == aux->nombre) {
            //The last node is the one I want to delete.
            //I have to destroy and free the last node and set the previous one to null.
            free(aux);
            if (length == 1) {
                list = NULL;
                return (list);
            } else {
                n = 1;
                aux = list;
                while (n < (length-1)) {
                    aux = aux->next;
                    n++;
                }
                aux->next = NULL;
                aux = NULL;
            }
        } else if (n == 1 && length > 1) {
            //List has more then one entry but I wish to delete the first one.
            list = aux->next;
            aux->next = NULL;
            free(aux);
            aux = NULL;
        } else if (nombre == aux->nombre) {
            //Node I want to remove is in middle of list.
            while (i < (n-1)) {
                aux1 = aux1->next;
                i++;
            }
            aux1->next = aux->next;
            free(aux);
            aux = NULL;
            aux1 = NULL;
        }
    }

    return (list);
}

void list_dump(list_t list) {
    list_t aux = list;

    while (aux != NULL) {
        if (aux != NULL) {
            printf("Vertice %u, posicion %u\n", aux->nombre, aux->index);
        }
        aux = aux->next;
    }
}

char* readline_from_stdin() {
    char* line = NULL; // String con lectura de stdin.

    // Línea tendrá MAX_LINE_LENGTH caracteres más '\n' más '\0'.
    line = calloc(MAX_LINE_LENGTH + 2, sizeof(char));
    assert(line != NULL); // Asegurar que se pudo pedir memoria.

    // Leer de stdin.
    if (fgets(line, MAX_LINE_LENGTH + 2, stdin) != NULL) {
        // Si hay un \n en el string, el input fue de tamaño correcto.
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0'; // Eliminar '\n'.
        } else {
            // Línea no cumple con formato. Libero memoria y devuelvo NULL.
            free(line);
            line = NULL;
        }
    }

    return (line);
}

par_t handle_p_line(char* line) {
    assert(line != NULL);
    char* subline = NULL; // Usada para guardar cada substring de la línea.
    char* ptr = NULL; // Necesaria para strtoul().
    int i = 0; /* Índice del substring de "line". Si llega a 4, línea ...
    no tiene formato requerido. */
    int char_i = 0; // Índice de los caracteres en "subline".
    bool es_num = true; // Si true, subline es un número.
    par_t par = NULL; // n vértices y m lados serán guardados en par.

    par = calloc(1, sizeof(struct _par_t));
    assert(par != NULL);

    /* Si en algún momento, uno de los 4 substrings de la línea no respeta su
     * formato requerido, par se libera y se la setea a NULL para que deje de
     * buscar.
     * i empieza en 0. Por cada substring, se aumenta en 1. Si llega a 4
     * y strsep no devuelve NULL, habían más cosas despues del cuarto substring.
     */
    while (par != NULL && i < 5 && (subline = strsep(&line, " ")) != NULL) {
        switch (i) {
            case 0:
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
                    par->l = strtoul(subline, &ptr, 10);
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
                    par->r = strtoul(subline, &ptr, 10);
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

    return (par);
}

par_t handle_e_line(char* line) {
    assert(line != NULL);

    char* subline = NULL; // Usada para guardar cada substring de la línea.
    char* ptr = NULL; // Necesaria para strtoul().
    int i = 0; /* Índice del substring de "line". Si llega a 3, línea ...
    no tiene formato requerido. */
    int char_i = 0; // Índice de los caracteres en "subline".
    bool es_num = true; // Si true, subline es un número.
    par_t par = NULL; // n vértices y m lados serán guardados en par.

    par = calloc(1, sizeof(struct _par_t));
    assert(par != NULL);

    /* Si en algún momento, uno de los 3 substrings de la línea no respeta su
     * formato requerido, par se libera y se la setea a NULL para que deje de
     * buscar.
     * i empieza en 0. Por cada substring, se aumenta en 1. Si llega a 3
     * y strsep no devuelve NULL, habían más cosas despues del tercer substring.
     */
    while (par != NULL && i < 4 && (subline = strsep(&line, " ")) != NULL) {
        switch (i) {
            case 0:
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
                    par->l = strtoul(subline, &ptr, 10);
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
                    par->r = strtoul(subline, &ptr, 10);
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
