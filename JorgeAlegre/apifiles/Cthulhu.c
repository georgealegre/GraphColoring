#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include <time.h>

#include "helpers.h"
#include "pila.h"
#include "list.h"
#include "Cthulhu.h"


struct _vertex_t{
    u32 nombre;
    u32 grado;
    u32 color;
};

struct _neighbours_t {
    bool* colors; /* Arreglo de tamaño del total de vecinos.
    La posición indica el color. El valor indica si el color está usado. */
    list_t list; // Lista de vecinos.
};

struct NimheSt{
    u32 nvertices; // Número de vertices en el grafo.
    u32 nlados; // Número de lados en el grafo.
    u32 ncolores; // Número de colores usados hasta el momento para el coloreo propio.
    u32* orden_actual; // String del orden del grafo.
    u32 delta_grande;
    u32* nvertices_color; // El valor en la posición "i" = vértices de color "i".
    VerticeSt* vertices; // Arreglo de vértices en orden original.
    neighbours_t* vecinos; // Arreglo de listas de vecinos de vértices.
};

void print_time(clock_t start){
    int msec;
    clock_t diff;

    diff = clock() - start;
    msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds.\n", msec/1000, msec%1000);
}

neighbours_t neighbours_empty() {
    neighbours_t neighbours = NULL;

    neighbours = calloc(1, sizeof(struct _neighbours_t));
    assert(neighbours != NULL);

    neighbours->list = list_empty();

    return (neighbours);
}

neighbours_t neighbours_init(neighbours_t neighbours, u32 size) {
    neighbours->colors = calloc(size, sizeof(bool));
    assert(neighbours->colors != NULL);

    return (neighbours);
}

neighbours_t neighbours_destroy(neighbours_t neighbours) {
    if (neighbours != NULL) {
        //Eliminamos la lista enlazada.
        if (neighbours->list != NULL) {
            neighbours->list = list_destroy(neighbours->list);
            assert(neighbours->list == NULL);
        }

        // Eliminamos bool vector.
        if (neighbours->colors != NULL) {
            free(neighbours->colors);
            neighbours->colors = NULL;
        }

        // Eliminamos estructura completa.
        free(neighbours);
        neighbours = NULL;
    }

    return (neighbours);
}

u32 neighbours_i(neighbours_t neighbours, u32 i) {
    return (list_index(neighbours->list, i));
}

neighbours_t neighbours_append(neighbours_t neighbours, u32 index, u32 nombre) {
    neighbours->list = list_append(neighbours->list, index, nombre);
    return (neighbours);
}

u32 neighbours_find_hole(neighbours_t neighbours, u32 grado) {
    u32 result = 0;
    u32 i = 0; // Iterador de vecinos.

    /* Índice 0 indica color 1, índice 1 indica color 2, ...,
    índice (size - 1) indica color "size". */

    for (i = 0; i < grado && !result; i++) {
        if (!neighbours->colors[i]) {
            result = i + 1;
        }
    }

    // Por cota superior de greedy.
    if (!result) {
        result = grado + 1;
    }

    return (result);
}

neighbours_t neighbours_update(NimheP G, u32 vertex) {
    u32 i = 0;
    u32 color_vecino = 0;

    for (i = 0; i < G->vertices[vertex].grado; i++) {
        color_vecino = G->vertices[neighbours_i(G->vecinos[vertex], i)].color;
        if (color_vecino && color_vecino <= G->vertices[vertex].grado) {
            // En palabras: si el color del vértice vecino "i" de "vertex"
            // es menor al grado de "vertex" y es mayor que 0, marcar el color
            // como usado.
            G->vecinos[vertex]->colors[color_vecino - 1] = true;
        }
    }

    return (G->vecinos[vertex]);
}

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
            printf("%u,", G->vertices[(neighbours_i(G->vecinos[posv], i))].nombre);
            i++;
        }

        // Imprimir con punto al último vecino.
        printf("%u.", G->vertices[(neighbours_i(G->vecinos[posv], i))].nombre);
    }
}

u32 max(u32 a, u32 b) {
    if (a <= b) return b;
    else return a;
}

NimheP NuevoNimhe() {
    char* line = NULL; // Línea leída.
    int read = 0; // Cantidad de caracteres leídos.
    bool leer = true; // True si seguimos leyendo, false de lo contrario.
    char* error_lectura = "Error en formato de entrada\n"; // Mensaje de error.
    u32 lvertice = 0; // Valor temp. del vértice izquierdo (v en "e v w").
    u32 rvertice = 0; // Valor temp. del vértice derecho (w en "e v w").
    u32 nlados_tmp = 1; /* Para saber cuántas veces leer lados de STDIN.
        Empieza con 1 para que el ciclo funcione. */
    u32 nlados_leidos = 0; // Para saber cuántos lados llevo leídos.
    bool existel = false; // True si vértice izquierdo ya fue procesado.
    bool exister = false; // True si vértice derecho ya fue procesado.
    u32 pos_rv = 0; // Indica la posición del vértice derecho leído.
    u32 pos_lv = 0; // Indica la posición del vértice izquierdo leído.
    u32 l = 0; // Iterador para buscar vértice izquierdo en arreglo de vértices.
    u32 r = 0; // Iterador para buscar vértice derecho en arreglo de vértices.
    par_t par = NULL; // Valores obtenidos de la lectura de cada línea.
    NimheP result = NULL; // Grafo que será devuelto por función.
    u32 pos_vertice_nuevo = 0; // Para saber hasta donde insertar vértice nuevo.
    u32 i = 0; // Para inicializar estructura de vecinos de vértices.
    u32 delta_grande = 0;

    clock_t start;

    start = clock();
    while (leer && nlados_leidos < nlados_tmp) {
        line = readline_from_stdin();
        printf("%s\n", line);
        if (line == NULL) {
            leer = false;
            printf("Readline devolvio NULL %s", error_lectura);
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
                        result->vecinos = calloc(result->nvertices, sizeof(neighbours_t));
                    } else {
                        leer = false;
                        printf("Mala lectura linea p %s", error_lectura);
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

                        // Agregar comentario
                        existel = false;
                        exister = false;
                        pos_rv = 0;
                        pos_lv = 0;
                        for (l = 0; l < pos_vertice_nuevo && !existel; l++) {
                            existel = (result->vertices[l].nombre == lvertice);
                            if (existel) pos_lv = l;
                        }
                        for (r = 0; r < pos_vertice_nuevo && !exister; r++) {
                            exister = (result->vertices[r].nombre == rvertice);
                            if (exister) pos_rv = r;
                        }

                        if (!existel && !exister) {
                            // Ambos vértices son nuevos.
                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = neighbours_empty();
                            result->vecinos[pos_vertice_nuevo] = neighbours_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo + 1, rvertice);

                            pos_vertice_nuevo++;

                            result->vertices[pos_vertice_nuevo].nombre = rvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = neighbours_empty();
                            result->vecinos[pos_vertice_nuevo] = neighbours_append(result->vecinos[pos_vertice_nuevo], pos_vertice_nuevo - 1, lvertice);

                            pos_vertice_nuevo++;
                        } else if (!existel && exister) {
                            // Solo cargué derecho
                            result->vertices[pos_vertice_nuevo].nombre = lvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = neighbours_empty();
                            result->vecinos[pos_vertice_nuevo] = neighbours_append(result->vecinos[pos_vertice_nuevo], pos_rv, rvertice);

                            // Agregar izquierdo como vecino de derecho.
                            result->vertices[pos_rv].grado++;
                            result->vecinos[pos_rv] = neighbours_append(result->vecinos[pos_rv], pos_vertice_nuevo, lvertice);

                            pos_vertice_nuevo++;
                        } else if (existel && !exister) {
                            // Solo cargué izquierdo
                            result->vertices[pos_vertice_nuevo].nombre = rvertice;
                            result->vertices[pos_vertice_nuevo].grado = 1;
                            result->vertices[pos_vertice_nuevo].color = 0;
                            result->vecinos[pos_vertice_nuevo] = neighbours_empty();
                            result->vecinos[pos_vertice_nuevo] = neighbours_append(result->vecinos[pos_vertice_nuevo], pos_lv, lvertice);

                            // Agregar derecho como vecino de izquierdo.
                            result->vertices[pos_lv].grado++;
                            result->vecinos[pos_lv] = neighbours_append(result->vecinos[pos_lv], pos_vertice_nuevo, rvertice);

                            pos_vertice_nuevo++;
                        } else {
                            // Ambos vértices ya existen. Solo setear vecinos.
                            result->vertices[pos_lv].grado++;
                            result->vertices[pos_rv].grado++;
                            result->vecinos[pos_lv] = neighbours_append(result->vecinos[pos_lv], pos_rv, rvertice);
                            result->vecinos[pos_rv] = neighbours_append(result->vecinos[pos_rv], pos_lv, lvertice);
                        }
                        nlados_leidos++;
                    } else {
                        leer = false;
                        printf("Mala lectura linea e %s", error_lectura);
                        if (!DestruirNimhe(result)) {
                            printf("Mala destrucción del grafo.\n");
                        }
                    }
                } else if ((result == NULL) && ((read == 1 && line[0] == 'c') || (line[0] == 'c' && line[1] == ' '))) {
                    // Comentario. No hacer nada.
                } else {
                    leer = false;
                    printf("No entro ni en e, ni en c ni en p. %s", error_lectura);
                    if (!DestruirNimhe(result)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                }
            } else if (read == 0) {
                if (nlados_leidos < nlados_tmp) {
                    printf("Numero de lados leidos no coincide con numero de lados del archivo %s", error_lectura);
                    if (!DestruirNimhe(result)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                }
                leer = false;
            }
            free(line);
            line = NULL;
        }
    }
    printf("Tarde en leer: \n");
    print_time(start);


    if (result != NULL) {
        for (i = 0; i < result->nvertices; i++) {
            result->vecinos[i] = neighbours_init(result->vecinos[i], result->vertices[i].grado);
            delta_grande = max(delta_grande, result->vertices[i].grado);
        }

        result->nvertices_color = calloc(delta_grande + 2, sizeof(u32));
        assert(result->nvertices_color != NULL);

        result->delta_grande = delta_grande;
    }

    return (result);
}

int DestruirNimhe(NimheP G) {
    int result = 1;
    u32 i = 0; // Iterador para liberar listas de vecinos.

    if (G != NULL) {
        if (G->vecinos != NULL) {
            for (i = 0; i < G->nvertices; i++) {
                G->vecinos[i] = neighbours_destroy(G->vecinos[i]);
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
        if (G->nvertices_color != NULL) {
            free(G->nvertices_color);
            G->nvertices_color = NULL;
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

    clock_t start;

    start = clock();

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
                w = neighbours_i(G->vecinos[v], x);
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
            if (G->vertices[i].color == G->vertices[neighbours_i(G->vecinos[i], x)].color) {
                result = 0;
            }
        }
    }

    printf("Chidos tardo: \n");
    print_time(start);

    return (result);
}

u32 Greedy(NimheP G) {
    u32 result = 0;
    u32 v = 0; // Iterador de vértices.
    u32 color = 0; // Color asignado a cada vértice en iteración.
    u32 c = 0; // Iterador de colores.

    clock_t start;

    start = clock();

    Descolorear(G);

    // Colorear primer vértice con color 1
    G->vertices[0].color = 1;

    for (v = 1; v < G->nvertices; v++) {
        // Por cada vértice a partir del segundo (index 1), recorrer su lista
        // de vecinos buscando el color más chico de los vértices de los vecinos
        // tal que este color sea distinto a todos los usados por los vecinos.

        // En pocas palabras, buscar el color más chico no usado por vecinos.

        // Encontrar color.
        G->vecinos[v] = neighbours_update(G, v);
        color = neighbours_find_hole(G->vecinos[v], G->vertices[v].grado);

        // Colorear vértice.
        G->vertices[v].color = color;

        // Aumentar arreglo de cantidad de vértices coloreados con "color".
        G->nvertices_color[color]++;
    }

    printf("Delta grande: %u\n", G->delta_grande);
    for (c = 1; c < G->delta_grande + 2; c++) {
        if (!G->nvertices_color[c]) result++;
    }

    printf("Greedy tardo:\n");
    print_time(start);

    return (result);
}

void OrdenNatural(NimheP G);

void OrdenWelshPowell(NimheP G);

void ReordenAleatorioRestringido(NimheP G);

void GrandeChico(NimheP G);

void ChicoGrande(NimheP G);

void Revierte(NimheP G);

void OrdenEspecifico(NimheP G,u32* x);
