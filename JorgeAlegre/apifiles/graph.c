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
#include "graph.h"
#include "rbtree.h"


struct _vertex_t{
    u32 nombre;
    u32 grado;
    u32 color;
};

struct _neighbours_t {
    bool* colors; /* Arreglo de tamaño del total de vecinos.
    La posición indica el color. El valor indica si el color está usado. */
    u32* neighbours; // Arreglo de vecinos.
    u32 size;
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
    printf("%d seconds %d milliseconds.\n", msec/1000, msec%1000);
}

neighbours_t neighbours_empty() {
    neighbours_t neighbours = NULL;

    neighbours = calloc(1, sizeof(struct _neighbours_t));
    assert(neighbours != NULL);

    return (neighbours);
}

neighbours_t neighbours_init(neighbours_t neighbours, u32 size) {
    neighbours->colors = calloc(size, sizeof(bool));
    assert(neighbours->colors != NULL);

    return (neighbours);
}

neighbours_t neighbours_destroy(neighbours_t neighbours) {
    if (neighbours != NULL) {
        //Eliminamos el arreglo de vecinos.
        if (neighbours->neighbours != NULL) {
            free(neighbours->neighbours);
            neighbours->neighbours = NULL;
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
    return (neighbours->neighbours[i]);
}

neighbours_t neighbours_append(neighbours_t neighbours, u32 index) {
    neighbours->neighbours = realloc(neighbours->neighbours, (neighbours->size + 1)*sizeof(u32));
    neighbours->neighbours[neighbours->size] = index;
    neighbours->size++;
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
        printf("%u.\n", G->vertices[(neighbours_i(G->vecinos[posv], i))].nombre);
    }
}

u32 max(u32 a, u32 b) {
    if (a <= b) return b;
    else return a;
}

NimheP agregar_vertices(NimheP G, u32 lvertice, bool existel, u32 pos_lv, u32 rvertice, bool exister, u32 pos_rv, u32* pos_vertice_nuevo) {
    if (!existel && !exister) {
        // Ambos vértices son nuevos.

        G->vertices[*pos_vertice_nuevo].nombre = lvertice;
        G->vertices[*pos_vertice_nuevo].grado = 1;
        G->vertices[*pos_vertice_nuevo].color = 0;
        G->vecinos[*pos_vertice_nuevo] = neighbours_empty();
        G->vecinos[*pos_vertice_nuevo] = neighbours_append(G->vecinos[*pos_vertice_nuevo], *pos_vertice_nuevo + 1);

        (*pos_vertice_nuevo)++;

        G->vertices[*pos_vertice_nuevo].nombre = rvertice;
        G->vertices[*pos_vertice_nuevo].grado = 1;
        G->vertices[*pos_vertice_nuevo].color = 0;
        G->vecinos[*pos_vertice_nuevo] = neighbours_empty();
        G->vecinos[*pos_vertice_nuevo] = neighbours_append(G->vecinos[*pos_vertice_nuevo], *pos_vertice_nuevo - 1);

        (*pos_vertice_nuevo)++;
    } else if (!existel && exister) {
        // Solo cargué derecho
        G->vertices[*pos_vertice_nuevo].nombre = lvertice;
        G->vertices[*pos_vertice_nuevo].grado = 1;
        G->vertices[*pos_vertice_nuevo].color = 0;
        G->vecinos[*pos_vertice_nuevo] = neighbours_empty();
        G->vecinos[*pos_vertice_nuevo] = neighbours_append(G->vecinos[*pos_vertice_nuevo], pos_rv);

        // Agregar izquierdo como vecino de derecho.
        G->vertices[pos_rv].grado++;
        G->vecinos[pos_rv] = neighbours_append(G->vecinos[pos_rv], *pos_vertice_nuevo);

        (*pos_vertice_nuevo)++;
    } else if (existel && !exister) {
        // Solo cargué izquierdo
        G->vertices[*pos_vertice_nuevo].nombre = rvertice;
        G->vertices[*pos_vertice_nuevo].grado = 1;
        G->vertices[*pos_vertice_nuevo].color = 0;
        G->vecinos[*pos_vertice_nuevo] = neighbours_empty();
        G->vecinos[*pos_vertice_nuevo] = neighbours_append(G->vecinos[*pos_vertice_nuevo], pos_lv);

        // Agregar derecho como vecino de izquierdo.
        G->vertices[pos_lv].grado++;
        G->vecinos[pos_lv] = neighbours_append(G->vecinos[pos_lv], *pos_vertice_nuevo);

        (*pos_vertice_nuevo)++;
    } else {
        // Ambos vértices ya existen. Solo setear vecinos.
        G->vertices[pos_lv].grado++;
        G->vertices[pos_rv].grado++;
        G->vecinos[pos_lv] = neighbours_append(G->vecinos[pos_lv], pos_rv);
        G->vecinos[pos_rv] = neighbours_append(G->vecinos[pos_rv], pos_lv);
    }

    return (G);
}

NimheP NuevoNimhe() {
    // Grafo.
    NimheP G = NULL; // Grafo que será devuelto por función.
    u32 delta_grande = 0; // Delta grande del grafo G.
    u32 nlados = 0; // Para saber cuántas veces leer lados de STDIN.

    // Proceso de lectura.
    char* line = NULL; // Línea leída.
    char* error_lectura = "Error en formato de entrada\n"; // Mensaje de error.
    u32 nlados_leidos = 0; // Para saber cuántos lados llevo leídos.

    // ¿Existe el vértice?
    rb_tree_t tree = NULL; // Arbol con vértices que leo.
    bool existel = false; // True si vértice izquierdo ya fue procesado.
    bool exister = false; // True si vértice derecho ya fue procesado.
    u32 pos_rv = 0; // Indica la posición del vértice derecho leído.
    u32 pos_lv = 0; // Indica la posición del vértice izquierdo leído.
    u32 lvertice = 0; // Valor temp. del vértice izquierdo (v en "e v w").
    u32 rvertice = 0; // Valor temp. del vértice derecho (w en "e v w").
    par_t par = NULL; // Valores obtenidos de la lectura de cada línea.
    u32 pos_vertice_nuevo = 0; // Para saber hasta donde insertar vértice nuevo.
    u32 i = 0; // Para inicializar estructura de vecinos de vértices.

    // Para guardar la hora de inicio.
    clock_t start;
    start = clock();

    // Primero descartamos líneas de comentario, si existen.
    for (;;) {
        if ((line = readline_from_stdin()) != NULL) {
            if (line[0] != 'c') {
                // Dejo línea lista para ser procesada por siguiente bloque.
                break;
            }
            free(line);
            line = NULL;
        } else {
            printf("%s", error_lectura);
            return NULL;
        }
    }

    // Después, procesamos línea "p edge n m".
    if (line != NULL) {
        if (line[0] == 'p') {
            if ((par = handle_p_line(line)) != NULL) {
                // Inicializamos grafo.
                G = calloc(1, sizeof(struct NimheSt));
                assert(G != NULL);

                // Obtengo datos leídos de línea.
                G->nvertices = get_l(par);
                G->nlados = get_r(par);
                nlados = get_r(par);
                G->ncolores = 0;

                // Ya terminé de usar datos de línea leída
                free(par);
                par = NULL;

                // Inicializar arreglo de vértices y vecinos.
                G->vertices = calloc(G->nvertices, sizeof(struct _vertex_t));
                assert(G->vertices != NULL);
                G->vecinos = calloc(G->nvertices, sizeof(neighbours_t));
                assert(G->vecinos != NULL);

                // Inicializo árbol de vértices leídos.
                tree = rb_new();
            } else {
                // Línea empieza con 'p' pero no está bien formateada.
                free(line);
                line = NULL;
                printf("%s", error_lectura);
                return NULL;
            }
        } else {
            // Siguiente línea despues de comentarios no empieza con 'p'.
            free(line);
            line = NULL;
            printf("%s", error_lectura);
            return NULL;
        }
        free(line);
        line = NULL;
    }

    // Finalmente, procesamos cada lado del grafo de entrada.
    if (G != NULL) {
        while (nlados_leidos < nlados) {
            if ((line = readline_from_stdin()) != NULL) {
                if (line[0] == 'e') {
                    if ((par = handle_e_line(line)) != NULL) {
                        // Obtengo nombre de vértices de línea leída.
                        lvertice = get_l(par);
                        rvertice = get_r(par);
                        free(par);
                        par = NULL;

                        // Reviso si vértice izquierdo ya existe.
                        existel = rb_exists(tree, lvertice);
                        pos_lv = 0;
                        // Si existe, decime donde.
                        // Si no existe, agregalo al árbol de vértices junto con su posición.
                        if (existel) pos_lv = rb_search(tree, lvertice);
                        else tree = rb_insert(tree, lvertice, pos_vertice_nuevo);

                        // Reviso si vértice derecho ya existe.
                        exister = rb_exists(tree, rvertice);
                        pos_rv = 0;
                        // Si existe, decime donde.
                        // Si no existe, su posición nueva depende de
                        // si vértice izquiero es nuevo o no.
                        if (exister) pos_rv = rb_search(tree, rvertice);
                        else if (existel) tree = rb_insert(tree, rvertice, pos_vertice_nuevo);
                        else tree = rb_insert(tree, rvertice, pos_vertice_nuevo + 1);

                        // Agrego vértices según corresponda.
                        G = agregar_vertices(G,
                                             lvertice,
                                             existel,
                                             pos_lv,
                                             rvertice,
                                             exister,
                                             pos_rv,
                                             &pos_vertice_nuevo);
                        nlados_leidos++;

                        free(line);
                    } else {
                        // Me quedan lados por leer pero hubo error de lectura.
                        printf("%s", error_lectura);
                        if (!DestruirNimhe(G)) {
                            printf("Mala destrucción del grafo.\n");
                        }
                        free(line);
                        line = NULL;
                        return NULL;
                    }
                } else {
                    // Me quedan lados por leer pero hubo error de lectura.
                    printf("%s", error_lectura);
                    if (!DestruirNimhe(G)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                    free(line);
                    line = NULL;
                    return NULL;
                }
            } else {
                // Me quedan lados por leer pero hubo error de lectura.
                printf("%s", error_lectura);
                if (!DestruirNimhe(G)) {
                    printf("Mala destrucción del grafo.\n");
                }
                return NULL;
            }
        }
    }

    // Liberar árbol.
    tree = rb_destroy(tree);
    assert(tree == NULL);

    // ¿Cuánto tardé en leer?
    printf("Tarde en leer: ");
    print_time(start);

    // DEBUG
    for (u32 i = 0; i < G->nvertices; i++) {
        printf("Vecinos del vértice %u: ", G->vertices[i].nombre);
        ImprimirVecinosDelVertice(G->vertices[i], G);
    }

    // Inicializo estructura de vecinos de cada vértice.
    // Obtengo delta grande del grafo.
    if (G != NULL) {
        for (i = 0; i < G->nvertices; i++) {
            G->vecinos[i] = neighbours_init(G->vecinos[i], G->vertices[i].grado);
            delta_grande = max(delta_grande, G->vertices[i].grado);
        }

        G->nvertices_color = calloc(delta_grande + 2, sizeof(u32));
        assert(G->nvertices_color != NULL);

        G->delta_grande = delta_grande;
    }

    return (G);
}

int DestruirNimhe(NimheP G) {
    u32 i = 0; // Iterador para liberar listas de vecinos.

    if (G != NULL) {
        if (G->vecinos != NULL) {
            for (i = 0; i < G->nvertices; i++) {
                G->vecinos[i] = neighbours_destroy(G->vecinos[i]);
                if (G->vecinos[i] != NULL) return 0;
            }
            free(G->vecinos);
            G->vecinos = NULL;
        }
        if (G->vertices != NULL) {
            free(G->vertices);
            G->vertices = NULL;
        }
        if (G->nvertices_color != NULL) {
            free(G->nvertices_color);
            G->nvertices_color = NULL;
        }
        free(G);
        G = NULL;
    }

    return (1);
}

u32 NumeroDeVertices(NimheP G) {
    if (G != NULL) return G->nvertices;
    else return 0;
}

u32 NumeroDeLados(NimheP G) {
    if (G != NULL) return G->nlados;
    else return 0;
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

    while (nvertices_coloreados < G->nvertices) {
        i = 0;
        while (G->vertices[i].color != 0 && i < G->nvertices) {
            i++;
        }
        G->vertices[i].color = 1;
        nvertices_coloreados++;
        stack = stack_push(stack, i);
        while (stack_size(stack)) {
            v = stack_first(stack);
            stack = stack_pop(stack);
            for (x = 0; x < G->vertices[v].grado ; x++) {
                w = neighbours_i(G->vecinos[v], x);
                if (G->vertices[w].color == 0) {
                    stack = stack_push(stack, w);
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

    printf("Chidos tardo: ");
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

    printf("Greedy tardo: ");
    print_time(start);

    for (u32 i = 0; i < G->nvertices; i++) {
        printf("Vecinos del vértice %u: ", G->vertices[i].nombre);
        ImprimirVecinosDelVertice(G->vertices[i], G);
    }

    return (result);
}

int compare_name(const void* vertex1, const void* vertex2) {
    VerticeSt *v1 = (VerticeSt *)vertex1;
    VerticeSt *v2 = (VerticeSt *)vertex2;
    return (v1->nombre - v2->nombre);
}

void OrdenNatural(NimheP G) {
    qsort(G->vertices, G->nvertices, sizeof(struct _vertex_t), compare_name);
}

void OrdenWelshPowell(NimheP G);

void ReordenAleatorioRestringido(NimheP G);

void GrandeChico(NimheP G);

void ChicoGrande(NimheP G);

void Revierte(NimheP G);

void OrdenEspecifico(NimheP G,u32* x);
