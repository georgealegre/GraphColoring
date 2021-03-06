/* Matemática Discreta II
 * Proyecto, primera parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "helpers.h"
#include "u32queue.h"
#include "Cthulhu.h"
#include "rbtree.h"

#define FACTOR_REALLOC 8

// Para acceder al grafo en las funciones de comparación.
static NimheP grafo = NULL;

typedef struct _neighbours_t {
    bool* colors;       /* Arreglo de tamaño del total de vecinos.
                         * posición + 1 = color. 
                         * El valor indica si el color está usado. */
    u32* neighbours;    // Arreglo de vecinos.
    u32 size;           // Cantidad de vecinos.
    u32 asize;          // Espacio disponible para agregar vecinos.
} *neighbours_t;

struct NimheSt{
    u32 nvertices;          // Número de vertices en el grafo.
    u32 nlados;             // Número de lados en el grafo.
    u32 ncolores;           // Número de colores usados hasta el momento.
    u32* orden;             // Orden actual del grafo.
    u32* orden_natural;     // Orden natural del grafo.
    u32 delta_grande;       // Delta grande del grafo.
    u32* nvertices_color;   // El valor en la pos. "i" = #vértices de color "i".
    VerticeSt* vertices;    // Arreglo de vértices en orden original.
    neighbours_t* vecinos;  // Arreglo de listas de vecinos de vértices.
};

inline neighbours_t
neighbours_empty()
{
    neighbours_t neighbours = NULL;

    neighbours = calloc(1, sizeof(struct _neighbours_t));
    assert(neighbours != NULL);

    return (neighbours);
}

inline neighbours_t
neighbours_init(neighbours_t neighbours, u32 size)
{
    // Una vez que sé cuántos vecinos tengo, crear arreglo con tamaño fijo.
    neighbours->colors = calloc(size, sizeof(bool));
    assert(neighbours->colors != NULL);

    return (neighbours);
}

inline neighbours_t
neighbours_destroy(neighbours_t neighbours)
{
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

    return (NULL);
}

inline u32
neighbours_i(neighbours_t neighbours, u32 i)
{
    // Devuelve la posición del iesimo vecino.
    return (neighbours->neighbours[i]);
}

inline neighbours_t
neighbours_append(neighbours_t neighbours, u32 index)
{
    if (neighbours->asize > neighbours->size) {
        // Tengo espacio para un vecino más.
        neighbours->neighbours[neighbours->size] = index;
        neighbours->size++;
    } else {
        // Pedir más memoria.
        neighbours->asize += FACTOR_REALLOC;
        neighbours->neighbours = realloc(neighbours->neighbours,
                (neighbours->asize)*sizeof(u32));
        // Guardar posición del vecino.
        neighbours->neighbours[neighbours->size] = index;
        neighbours->size++;
    }

    return (neighbours);
}

inline u32
neighbours_find_hole(neighbours_t neighbours, u32 grado)
{
    /* Índice 0 indica color 1, índice 1 indica color 2, ...,
     * índice (grado - 1) indica color "grado". */

    for (u32 i = 0; i < grado; i++) {
        // i es iterador de vértices.
        if (!neighbours->colors[i])
            // Si el color no esta usado, devuelvo el color.
            return (i + 1);
    }

    // Por cota superior de greedy.
    return (grado + 1);
}

inline neighbours_t
neighbours_update(NimheP G, u32 vertex)
{
    u32 color_vecino = 0;

    // Limpiar información vieja.
    memset(G->vecinos[vertex]->colors, 0,
           (G->vertices[vertex].grado)*sizeof(bool));

    for (u32 i = 0; i < G->vertices[vertex].grado; i++) {
        // i es iterador de vértices.
        // Obtener el color del iésimo vecino.
        color_vecino = G->vertices[neighbours_i(G->vecinos[vertex], i)].color;
        if (color_vecino && color_vecino <= G->vertices[vertex].grado) {
            // En palabras: si el color del iesimo vecino de "vertex"
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
    u32 posv = 0; // Para buscar la posición de "x".

    if (G != NULL) {
        for (posv = 0; posv < G->nvertices; posv++) {
            // Buscar la posición del vértice "x".
            if ((G->vertices[posv]).nombre == x.nombre) {
                break;
            }
        }

        // Imprimir con comma todos menos el último vecino.
        for (u32 i = 0; i < G->vertices[posv].grado - 1; i++) {
            // i es iterador de vecinos.
            // Imprime el nombre del vertice en la posicion de cada uno
            // de los vecinos del vertice de entrada.
            printf("%u,",
                   G->vertices[(neighbours_i(G->vecinos[posv], i))].nombre);
        }

        // Imprimir con punto al último vecino.
        printf("%u.\n",
               G->vertices[(neighbours_i(G->vecinos[posv],
                                         G->vertices[posv].grado - 1))].nombre);
    }
}

inline u32
max(u32 a, u32 b)
{
    if (a <= b) return b;
    else return a;
}

NimheP
agregar_vertices(NimheP G,
                        u32 lvertice,
                        bool existel,
                        u32 pos_lv,
                        u32 rvertice,
                        bool exister,
                        u32 pos_rv,
                        u32* pos_v_nuevo)
{
    if (!existel && !exister) {
        // Ambos vértices son nuevos.

        G->vertices[*pos_v_nuevo].nombre = lvertice;
        G->vertices[*pos_v_nuevo].grado = 1;
        G->vertices[*pos_v_nuevo].color = 0;
        G->vecinos[*pos_v_nuevo] = neighbours_empty();
        G->vecinos[*pos_v_nuevo] = neighbours_append(G->vecinos[*pos_v_nuevo],
                                                     *pos_v_nuevo + 1);

        // Cargar el derecho en la posición correspondiente.
        (*pos_v_nuevo)++;

        G->vertices[*pos_v_nuevo].nombre = rvertice;
        G->vertices[*pos_v_nuevo].grado = 1;
        G->vertices[*pos_v_nuevo].color = 0;
        G->vecinos[*pos_v_nuevo] = neighbours_empty();
        G->vecinos[*pos_v_nuevo] = neighbours_append(G->vecinos[*pos_v_nuevo],
                                                     *pos_v_nuevo - 1);

        (*pos_v_nuevo)++;
    } else if (!existel && exister) {
        // Solo cargué derecho
        G->vertices[*pos_v_nuevo].nombre = lvertice;
        G->vertices[*pos_v_nuevo].grado = 1;
        G->vertices[*pos_v_nuevo].color = 0;
        G->vecinos[*pos_v_nuevo] = neighbours_empty();
        G->vecinos[*pos_v_nuevo] = neighbours_append(G->vecinos[*pos_v_nuevo],
                                                     pos_rv);

        // Agregar izquierdo como vecino de derecho.
        G->vertices[pos_rv].grado++;
        G->vecinos[pos_rv] = neighbours_append(G->vecinos[pos_rv],
                                               *pos_v_nuevo);

        (*pos_v_nuevo)++;
    } else if (existel && !exister) {
        // Solo cargué izquierdo
        G->vertices[*pos_v_nuevo].nombre = rvertice;
        G->vertices[*pos_v_nuevo].grado = 1;
        G->vertices[*pos_v_nuevo].color = 0;
        G->vecinos[*pos_v_nuevo] = neighbours_empty();
        G->vecinos[*pos_v_nuevo] = neighbours_append(G->vecinos[*pos_v_nuevo],
                                                     pos_lv);

        // Agregar derecho como vecino de izquierdo.
        G->vertices[pos_lv].grado++;
        G->vecinos[pos_lv] = neighbours_append(G->vecinos[pos_lv], 
                                               *pos_v_nuevo);

        (*pos_v_nuevo)++;
    } else {
        // Ambos vértices ya existen. Solo agregar vecinos.
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
    u32 nlados_leidos = 0; // Para saber cuántos lados llevo leídos.
    u32 nvertices_leidos = 0; // Para ver vértices aislados.

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
                G->ncolores = 0;

                // Obtengo datos leídos de línea.
                G->nvertices = get_l(par);
                G->nlados = get_r(par);
                nlados = get_r(par);
                // Se setea a la cantidad de vértices que tiene el grafo.
                // A medida que leo un vértice nuevo, decremento por 1.
                // Al final, nvertices_leidos debería valer 0.
                // Sino, error en formato de entrada.
                nvertices_leidos = get_l(par);

                // Ya terminé de usar datos de línea leída
                free(par);
                par = NULL;

                // Inicializar arreglo de vértices y vecinos.
                G->vertices = calloc(G->nvertices, sizeof(struct _vertex_t));
                assert(G->vertices != NULL);
                G->vecinos = calloc(G->nvertices, sizeof(neighbours_t));
                assert(G->vecinos != NULL);

                // Inicializo orden
                G->orden = calloc(G->nvertices, sizeof(u32));
                assert(G->orden != NULL);
                for (i = 0; i < G->nvertices; i++)
                    G->orden[i] = i;
	
                // Inicializo árbol de vértices leídos.
                tree = rb_new();
            } else {
                // Línea empieza con 'p' pero no está bien formateada.
                free(line);
                line = NULL;
                return NULL;
            }
        } else {
            // Siguiente línea despues de comentarios no empieza con 'p'.
            free(line);
            line = NULL;
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
                        if (existel) {
                            // Si existe, decime donde.
                            pos_lv = rb_search(tree, lvertice);
                        } else {
                            // Si no existe, agregalo al árbol de vértices
                            // junto con su posición nueva.
                            // Decrementar por 1 nvertices leidos.
                            nvertices_leidos--;
                            tree = rb_insert(tree, lvertice, pos_vertice_nuevo);
                        }

                        // Reviso si vértice derecho ya existe.
                        exister = rb_exists(tree, rvertice);
                        if (exister) {
                            // Si existe, decime donde.
                            pos_rv = rb_search(tree, rvertice);
                        } else if (existel) {
                            // Si no existe, su posición nueva depende de
                            // si vértice izquiero es nuevo o no.
                            tree = rb_insert(tree, rvertice, pos_vertice_nuevo);
                            nvertices_leidos--;
                        } else {
                            tree = rb_insert(tree, rvertice, pos_vertice_nuevo + 1);
                            nvertices_leidos--;
                        }

                        // Agrego vértices según corresponda.
                        G = agregar_vertices(G,
                                             lvertice,
                                             existel,
                                             pos_lv,
                                             rvertice,
                                             exister,
                                             pos_rv,
                                             &pos_vertice_nuevo);
                        // Aumentamos por 1 la cantidad de vértices leidos.
                        nlados_leidos++;

                        // Borramos línea.
                        free(line);
                    } else {
                        // Me quedan lados por leer pero hubo error de lectura.
                        if (!DestruirNimhe(G)) {
                            printf("Mala destrucción del grafo.\n");
                        }
                        free(line);
                        line = NULL;
                        return NULL;
                    }
                } else {
                    // Me quedan lados por leer pero hubo error de lectura.
                    if (!DestruirNimhe(G)) {
                        printf("Mala destrucción del grafo.\n");
                    }
                    free(line);
                    line = NULL;
                    return NULL;
                }
            } else {
                // Me quedan lados por leer pero hubo error de lectura.
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

    // Ver vertices aislados.
    if (nvertices_leidos) {
        if (!DestruirNimhe(G)) {
            printf("Mala destrucción del grafo.\n");
        }
        return NULL;
    }

    // Obtengo delta grande del grafo.
    if (G != NULL) {
        for (i = 0; i < G->nvertices; i++) {
            G->vecinos[i] = neighbours_init(G->vecinos[i], G->vertices[i].grado);
            delta_grande = max(delta_grande, G->vertices[i].grado);
        }

        /* G->nvertices_color[0] = # vértices no coloreados.
         * G->nvertices_color[i] = # vértices coloreados con i.
         * Tamaño es delta grande + 2 para guardar espacio para color 0
         * y por cota superior de Greedy (delta grande + 1). */
        G->nvertices_color = calloc(delta_grande + 2, sizeof(u32));
        assert(G->nvertices_color != NULL);

        G->delta_grande = delta_grande;
    }
    
    // Inicializo variable global.
    grafo = G;

    // Guardo orden natural para uso repetido.
    OrdenNatural(G);
    G->orden_natural = calloc(G->nvertices, sizeof(u32));
    assert(G->orden_natural != NULL);
    for (u32 i = 0; i < G->nvertices; i++)
        G->orden_natural[i] = G->orden[i];

    // Inicializo semilla.
    srand((G->nvertices * G->nlados) + G->vertices[G->orden[1]].nombre);

    return (G);
}

int DestruirNimhe(NimheP G) {
    u32 i = 0; // Iterador para liberar listas de vecinos.

    if (G != NULL) {
        // Destruir estructura de vecinos para cada vértice.
        if (G->vecinos != NULL) {
            for (i = 0; i < G->nvertices; i++) {
                G->vecinos[i] = neighbours_destroy(G->vecinos[i]);
                if (G->vecinos[i] != NULL) return 0;
            }
            free(G->vecinos);
            G->vecinos = NULL;
        }
        // Destruir vértices.
        if (G->vertices != NULL) {
            free(G->vertices);
            G->vertices = NULL;
        }
        // Destruir orden del grafo.
        if (G->orden != NULL) {
            free(G->orden);
            G->orden = NULL;
        }
        // Destruir orden natural.
        if (G->orden_natural != NULL) {
            free(G->orden_natural);
            G->orden_natural = NULL;
        }
        // Destruir arreglo de colores usados.
        if (G->nvertices_color != NULL) {
            free(G->nvertices_color);
            G->nvertices_color = NULL;
        }
        grafo = NULL;
        // Destruir grafo.
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
    if (G != NULL) return G->nvertices_color[i];
    else return 0;
}

u32 ImprimirVerticesDeColor(NimheP G, u32 i) {
    u32 result = 0;

    if (G != NULL) {
        for (u32 j = 0; j < G->nvertices; j++) {
            if (G->vertices[j].color == i) {
                if (result == 0) {
                    printf("Vertices de Color %u: %u", i,G->vertices[j].nombre);
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
    if (G != NULL) return G->ncolores;
    else return 0;
}

VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i) {
    return G->vertices[G->orden[i]];
}

VerticeSt IesimoVecino(NimheP G, VerticeSt x, u32 i) {
    u32 posx = 0; // Para guardar posición de vertice x.

    // Encontrar posición del vertice x.
    for (u32 i = 0; i < G->nvertices; i++) {
        if (G->vertices[i].nombre == x.nombre) {
            posx = i;
            break;
        }
    }
    // Devolver iésimo vecino del vértice x.
    return G->vertices[neighbours_i(G->vecinos[posx], i)];
}


/***********************************************************
 *                                                         *
 *                  Funciones de coloreo.                  *
 *                                                         *
 ***********************************************************/

int coloreo_es_propio(NimheP G) {
    u32 color_v = 0; // Para guardar el color de cada vértice.

    for (u32 i = 0; i < G->nvertices; i++) {
        // i es iterador de vértices.
        color_v = G->vertices[i].color;
        for (u32 j = 0; j < G->vertices[i].grado; j++) {
            // j es iterador de vecinos.
            if (color_v == G->vertices[neighbours_i(G->vecinos[i], j)].color)
                // Si tienen el mismo color, no es propio.
                return (0);
        }
    }

    return (1);
}

int Chidos(NimheP G) {
    u32 i = 0; // Iterador de vértices en el grafo.
    u32 x = 0; // Iterador de vecinos del vértice.
    u32 nvertices_coloreados = 0; // Cuantos vértices colorié.
    u32 v; // Posición del vértice sobre el cual trabajaremos.
    u32 w; // Posición del vecino del "v".
    u32queue_t queue = queue_empty(); // Cola para guardar vecinos del vértice.

    // Mientras tenga vértices para colorear.
    while (nvertices_coloreados < G->nvertices) {
        // Buscar primer vértice no coloreado.
        i = 0;
        while (G->vertices[i].color != 0) {
            i++;
        }

        // Colorearlo con 1 y aumentar número de vértices coloreados.
        G->vertices[i].color = 1;
        nvertices_coloreados++;

        // Encolar vértice (i.e., su posición).
        queue = queue_enqueue(queue, i);

        // Mientras la cola tenga vértices.
        while (!queue_is_empty(queue)) {
            // Sacar el primero de la cola.
            v = queue_first(queue);
            queue = queue_dequeue(queue);

            // Por cada vecino del vértice.
            for (x = 0; x < G->vertices[v].grado ; x++) {
                w = neighbours_i(G->vecinos[v], x);

                // Si no esta coloreado, colorearlo y agregarlo a cola.
                if (G->vertices[w].color == 0) {
                    queue = queue_enqueue(queue, w);
                    G->vertices[w].color = 3 - G->vertices[v].color;
                    nvertices_coloreados++;
                }
            }
        }
    }

    // Liberar memoria usada por cola.
    queue = queue_destroy(queue);
    assert(queue == NULL);

    return (coloreo_es_propio(G));
}

u32 Greedy(NimheP G) {
    u32 vactual = 0; // Posición del vértice sobre el cual se esta trabajando.
    u32 color = 0; // Color asignado a cada vértice en iteración.

    // Descolorear grafo.
    for (u32 v = G->nvertices; v != 0; v--)
        G->vertices[v-1].color = 0;
   
    // Limpiar colores usados.
    memset(G->nvertices_color, 0, (G->delta_grande + 2)*sizeof(u32));

    // Colorear primer vértice con color 1
    G->vertices[G->orden[0]].color = 1;
    G->nvertices_color[1]++;
   
    for (u32 v = 1; v < G->nvertices; v++) {
        // v es iterador de vértices.
        vactual = G->orden[v];

        // Encontrar color más chico no usado por vecinos.
        G->vecinos[vactual] = neighbours_update(G, vactual);
        color = neighbours_find_hole(G->vecinos[vactual],
                                     G->vertices[vactual].grado);

        // Colorear vértice.
        G->vertices[vactual].color = color;

        // Aumentar por 1 cantidad de vértices coloreados con "color".
        G->nvertices_color[color]++;
    }

    // Contar cuántos colores fueron usados.
    for (u32 c = G->delta_grande + 1; c != 1; c--) {
        if (G->nvertices_color[c] != 0) {
            // De arriba a abajo, buscar primer color usado.
            // Esa es la cantidad de colores usados.
            G->ncolores = c;
            return (c);
        }
    }


    return (G->ncolores);
}


/***********************************************************
 *                                                         *
 *              Funciones de ordenación.                   *
 *                                                         *
 ***********************************************************/

int cmp_nat(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 ln = grafo->vertices[l].nombre;
    u32 rn = grafo->vertices[r].nombre;

    // Si el nombre del vértice izquierdo es menor al nombre del vértice derecho
    // está bien ordenado.
    if (ln < rn) return -1;
    else if (ln > rn) return 1;

    return 0;
}

void OrdenNatural(NimheP G) {
    qsort(G->orden, G->nvertices, sizeof(u32), cmp_nat);
}

int cmp_wp(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 dl = grafo->vertices[l].grado;
    u32 dr = grafo->vertices[r].grado;

    // Si el grado del vértice derecho es menor al grado del vértice izquierdo
    // está bien ordenado.
    if (dl > dr) return -1;
    else if (dl < dr) return 1;

    return 0;
}

void OrdenWelshPowell(NimheP G) {
    qsort(G->orden, G->nvertices, sizeof(u32), cmp_wp);
}

static inline void swap(u32* array, u32 l, u32 r) {
    u32 temp = array[l];
    array[l] = array[r];
    array[r] = temp;
}

static u32* tmp;

int cmp_rar(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 cl = grafo->vertices[l].color-1;
    u32 cr = grafo->vertices[r].color-1;

    if (tmp[cl] < tmp[cr]) return -1;
    else if (tmp[cl] > tmp[cr]) return 1;

    return 0;
}

void ReordenAleatorioRestringido(NimheP G) {
    u32* rnd_colors = calloc(G->ncolores, sizeof(u32));
    tmp = calloc(G->ncolores, sizeof(u32));

    // Inicializar arreglo de colores aleatorios.
    for (u32 i = 0; i < G->ncolores; i++)
        rnd_colors[i] = i + 1;

    // Randomizar arreglo.
    for (u32 i = 0; i < G->ncolores; i++)
        swap(rnd_colors, i, (rand() % G->ncolores));

    for (u32 i = 0; i < G->ncolores; i++) {
        tmp[rnd_colors[i] - 1] = i;
    }

    free(rnd_colors);
    rnd_colors = NULL;

    // En tmp tengo, en la posición "i", su orden.
    // i.e., tmp[4] == 2 y tmp[7] == 9 significa que el color 5 (4+1) está primero
    // que el color 8 (7+1) ya que 2 < 9.


    qsort(G->orden, G->nvertices, sizeof(u32), cmp_rar);

    // Liberar memoria usada.
    free(tmp);
    tmp = NULL;
}

int cmp_gc(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 ncl = grafo->nvertices_color[grafo->vertices[l].color];
    u32 ncr = grafo->nvertices_color[grafo->vertices[r].color];
    u32 cl = grafo->vertices[l].color;
    u32 cr = grafo->vertices[r].color;

    if (ncl > ncr) return -1;
    else if (ncl < ncr) return 1;
    else if (cl < cr) return -1;
    else if (cl > cr) return 1;
    else return 0;
}

void GrandeChico(NimheP G) {
    qsort(G->orden, G->nvertices, sizeof(u32), cmp_gc);
}

int cmp_cg(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 ncl = grafo->nvertices_color[grafo->vertices[l].color];
    u32 ncr = grafo->nvertices_color[grafo->vertices[r].color];
    u32 cl = grafo->vertices[l].color;
    u32 cr = grafo->vertices[r].color;

    if (ncl < ncr) return -1;
    else if (ncl > ncr) return 1;
    else if (cl < cr) return -1;
    else if (cl > cr) return 1;
    else return 0;
}

void ChicoGrande(NimheP G) {
    qsort(G->orden, G->nvertices, sizeof(u32), cmp_cg);
}

int cmp_r(const void* left, const void* right) {
    u32 l = *(u32*) left;
    u32 r = *(u32*) right;
    u32 cl = grafo->vertices[l].color;
    u32 cr = grafo->vertices[r].color;

    if (cl > cr) return -1;
    else if (cl < cr) return 1;

    return 0;
}

void Revierte(NimheP G) {
    qsort(G->orden, G->nvertices, sizeof(u32), cmp_r);
}

void OrdenEspecifico(NimheP G, u32* x) {
    rb_tree_t tree = rb_new(); // Para saber si "x" cumple PRE condición.

    /* Checkear que "x" sean todos los números entre 0 y n-1 sin repetir.
     * Si no cumple esta condición, no modificar el orden de G. */
    for (u32 i = 0; i < G->nvertices; i++) {
        // Ver si números en "x" existen en árbol.
        // Agregar números en "x" a árbol.
        // Si existen, error con PRE condición.
        if (!rb_exists(tree, x[i]) && (x[i] < G->nvertices)) {
            tree = rb_insert(tree, x[i], x[i]);
        } else {
            tree = rb_destroy(tree);
            return;
        }
    }

    // Si llegué hasta acá, "x" cumple PRE condición.
    // Eliminar memoria usada por árbol.
    tree = rb_destroy(tree);

        
    // Ordenar con orden de "x".
    for (u32 i = 0; i < G->nvertices; i++) {
        G->orden[i] = G->orden_natural[x[i]];
    }
}
