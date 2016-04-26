#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "helpers.h"
#include "pila.h"
#include "list.h"
#include "Cthulhu.h"


/*
Ver lo de no tener una lista con los vertices y solo recorrer el grafo hasta
new vertex position
*/

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
                        result->vecinos = calloc(result->nvertices, sizeof(list_t));
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
                        free(par);
                        par = NULL;

                        existel = list_exists(vertices_cargados, lvertice);
                        exister = list_exists(vertices_cargados, rvertice);

                        if (!existel && !exister) {
                            // Ambos vértices son nuevos.
                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_empty();
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo + 1, rvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, lvertice);

                            pos_vertice_nuevo++;

                            result->vertices[pos_vertice_nuevo].nombre = rvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_empty();
                            result->vecinos[pos_vertice_nuevo] = list_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo - 1, lvertice);
                            vertices_cargados = list_append(vertices_cargados, pos_vertice_nuevo, rvertice);

                            pos_vertice_nuevo++;
                        } else if (!existel && exister) {
                            // Solo cargué derecho
                            pos_rv = list_search(vertices_cargados, rvertice);

                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = list_empty();
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
                            result->vecinos[pos_vertice_nuevo] = list_empty();
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
                if (nlados_leidos < nlados_tmp) {
                    printf("%s", error_lectura);
                    if (!DestruirNimhe(result)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                }
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

VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i);

VerticeSt IesimoVecino(NimheP G, VerticeSt x, u32 i);

void Descolorear(NimheP G) {
    u32 i = 0;

    if (G != NULL) {
        for (i = 0; i < G->nvertices; i++) {
            G->vertices[i].color = 0;
        }
    }
}

int Chidos(NimheP G) {
    int result = 1;
    u32 i = 0; // Iterador de vértices en el grafo.
    u32 x = 0; // Iterador de vecinos del vértice.
    u32 nvertices_coloreados = 0; // Cuantos vértices colorie.
    u32 v; // Posición del vértice sobre el cual trabajaremos.
    u32 w; // Posición del vecino del "v".
    pila_t stack = stack_empty();

    Descolorear(G);
    while (nvertices_coloreados < G->nvertices) {
        i = 0;
        while (G->vertices[i].color != 0 && i < G->nvertices) {
            i++;
        }
        G->vertices[i].color = 1;
        nvertices_coloreados++;
        stack = stack_push(stack, i); //crear Queue con G->vertices[i] como unico elemento.
        while (stack_size(stack)) {
            v = stack_first(stack);
            stack = stack_pop(stack);
            for (x = 0; x < G->vertices[v].grado ; x++) {
                w = list_index(G->vecinos[v], x);
                if (G->vertices[w].color == 0) {
                    //push queue (vecino)
                    stack = stack_push(stack, w);
                    //color del vecino = 3 - color de v
                    G->vertices[w].color = 3 - G->vertices[v].color;
                    nvertices_coloreados++;
                }
            }
        }
    }

    // Liberar memoria usada por stack.
    stack = stack_destroy(stack);
    assert(stack == NULL);

    // Ver que el coloreo sea propio.
    for (i = 0; i < G->nvertices && result; i++) {
        for (x = 0; x < G->vertices[i].grado && result; x++) {
            if (G->vertices[i].color == G->vertices[list_index(G->vecinos[i], x)].color) {
                result = 0;
            }
        }
    }

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
