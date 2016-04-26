// Autores:
// Alegre, Jorge Facundo <facu.alegre@gmail.com>
// Barraco, Ramiro <ramabarraco@gmail.com>

#include <stdint.h>

#define MAX_LINE_LENGTH 80

typedef uint32_t u32;
typedef struct NimheSt *NimheP;
typedef struct _vertex_t VerticeSt;
typedef struct _node_t  *list_t;
typedef struct _par_t* par_t;


u32 ColorDelVertice(VerticeSt x);
/* Devuelve el color con el que está coloreado el vértice x.
 * Si el vértice no esta coloreado, devuelve 0.
 */

u32 GradoDelVertice(VerticeSt x);
/* Devuelve el grado del vértice x. */

u32 NombreDelVertice(VerticeSt x);
/* Devuelve el nombre real del vértice x. */

void ImprimirVecinosDelVertice(VerticeSt x, NimheP G);
/* Imprime en standard output una lista de los vecinos de x.
 * La lista debe estar formada por los nombres reales de los vertices,
 * separados por comas, con un punto al final de la lista.
 * Ejemplo:
 * VecinosDelVertice(x,G);
 * debe producir:
 * 1,7,10,333,819092.
 * (asumiendo que esos son los vecinos de x). No es necesario que la lista esté
 * ordenada en ningún orden particular.
 */

NimheP NuevoNimhe();
/* La función aloca memoria, inicializa lo que haya que inicializar de una
 * estructura NimheSt y devuelve un puntero a ésta. Ademas lee un grafo desde
 * standard input en el formato especificado al principio de este documento y
 * llena esa estructura NimheSt con esos datos. Observación: como estan
 * haciendo estas cosas al mismo tiempo, al leer la entrada ya sabrán cuantos
 * vértices y lados tiene el grafo antes de tener que leer todos los lados, por
 * lo que pueden eficientar la inicializaci ́on de la estructura.
 * Ademas de cargar el grafo, asigna el “color” 0 a cada vertice para indicar
 * que están todos no coloreados.
 * Como dijimos, la función debe leer desde standard input. En general le
 * daremos de comer una serie de lineas en ese formato, a mano, o bien usaremos
 * en standard input el redireccionador “<” para redireccionar a un archivo que
 * sea de esa forma.
 * En caso de error, la función devolverá un puntero a NULL.
 * (errores posibles pueden ser falla en alocar memoria, pero también que el
 * formato de entrada no sea válido, por ejemplo, que la primera linea sin
 * comentarios no empieze con p o que n∗ ̸= n (ver en el item de formato de
 * entrada que significa esto)
 */

int DestruirNimhe(NimheP G);
/* Destruye G y libera la memoria alocada.
 * Retorna 1 si todo anduvo bien y 0 si no.
 */

u32 NumeroDeVertices(NimheP G);
/* Devuelve el número de vértices del grafo G. */

u32 NumeroDeLados(NimheP G);
/* Devuelve el número de lados del grafo G. */

u32 NumeroVerticesDeColor(NimheP G, u32 i);
/* Retorna el número de vértices de color i.
 * (si i = 0 devuelve el número de vertices no coloreados).
 */

u32 ImprimirVerticesDeColor(NimheP G, u32 i);
/* Imprime una linea que dice “Vertices de Color i:” y a continuacion una lista
 * de los vertices que tienen el color i, separados por comas y con un punto
 * luego del último color.
 * (si i = 0 esta lista será la lista de vertices no coloreados)
 * Por ejemplo: Vertices de Color 3: 15,17,1.
 * Los vertices no tienen porque estar ordenados.
 * Si no hay vertices de color i debe imprimir “No hay vertices de color i”.
 * Retorna el número de vertices de color i.
 */

u32 CantidadDeColores(NimheP G);
/* Devuelve la cantidad de colores usados en el coloreo de G. */

VerticeSt IesimoVerticeEnElOrden(NimheP G, u32 i);
/* Devuelve el vértice numero i en el orden guardado en ese momento en G.
 * (el índice 0 indica el primer vértice, el índice 1 el segundo, etc).
 */

VerticeSt IesimoVecino(NimheP G, VerticeSt x, u32 i);
/* Devuelve el vécino numero i de x en el orden en que lo tengan guardado uds.
 * en G. (el orden es irrelevante, lo importante es que de esta forma podemos
 * pedir externamente la lista de vecinos) (el índice 0 indica el primer
 * vértice, el índice 1 el segundo, etc)
 * Nota acerca de la eficiencia: Obviamente si deciden guardar la estructura
 * del orden de G o la de los vécinos en un array, estas últimas dos funciones
 * serán mas eficientes que si deciden usar por ejemplo pointers al “siguiente”
 * vécino o vértice en el orden, o alguna otra estructura para la cual para
 * determinar el íesimo elemento haya que recorrer toda la estructura desde el
 * principio, pero para estas dos funciones NO se tienen que preocupar por la
 * eficiencia, pues estan sólo para testear.
 * Es decir, usen la estructura interna que mas les parezca conveniente para
 * correr Greedy, ordenar repetidamente, etc, y no se preocupen si estas dos
 * últimas funciones demoran mas o menos, porque sólo la usaremos en mains para
 * testear que su estructura interna este guardando bien los vecinos y el orden,
 * usando grafos relativamente chicos.
 * Nota II medio irrelevante: esta función la podría haber puesto en la
 * subsección anterior, pero la puse aqui porque es la función que me permite
 * extraer los lados de G, junto con la función anterior a esta. Como no les
 * estoy pidiendo una estructura “Lados” ni que guarden los lados, no les puedo
 * pedir que me den el íesimo lado (aunque algunos proyectos podrían hacerlo
 * pues quizas usen una lista de lados) pero hagan lo que hagan, deben tener una
 * forma de buscar los vecinos de un vértice o no podrían correr Greedy, por eso
 * pido esta función.
 */

int Chidos(NimheP G);
/* Devuelve 1 si χ(G) = 2, 0 si no. */

u32 Greedy(NimheP G);
/* Corre greedy en G con el orden interno que debe estar guardado de alguna
 * forma dentro de G. Devuelve el numero de colores que se obtiene. */

void OrdenNatural(NimheP G);
/* Ordena los vertices en orden creciente de sus “nombres” reales.
 * (recordar que el nombre real de un vértice es un u32)
 */

void OrdenWelshPowell(NimheP G);
/* Ordena los vertices de G de acuerdo con el orden Welsh-Powell, es decir, con
 * los grados en orden no creciente.
 */

void ReordenAleatorioRestringido(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1, W2
 * los coloreados con 2, etc, entonces esta función ordena los vertices poniendo
 * primero los vertices de Wj1 (en algún orden) luego los de Wj2
 * (en algún orden), etc, donde j1,j2,... son aleatorios (pero distintos).
 */

void GrandeChico(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1,
 * W2 los coloreados con 2, etc, entonces esta función ordena los vertices
 * poniendo primero los vertices de Wj1 (en algún orden) luego los de Wj2
 * (en algún orden), etc, donde j1, j2, ... son tales
 * que |Wj1 | ≥ |Wj2 | ≥ ... ≥ |Wjr | .
 */

void ChicoGrande(NimheP G);
/* Idem que el anterior excepto que ahora el orden es tal
 * que |Wj1 | ≤ |Wj2 | ≤ ... ≤ |Wjr | .
 */

void Revierte(NimheP G);
/* Si G esta coloreado con r colores y W1 son los vertices coloreados con 1,
 * W2 los coloreados con 2, etc, entonces esta función ordena los vertices
 * poniendo primero los vertices de Wr (en algún orden) luego los de Wr−1
 * (en algún orden), etc.
 */

void OrdenEspecifico(NimheP G,u32* x);
/* La imagen de x deben ser todos los numeros entre 0 y n − 1, sin repetir.
 * La función primero chequea que x cumpla esa condición. Si no lo hace, la
 * función deja el orden de G como estaba.
 * Si x cumple la condición, entonces la función ordena los vertices con
 * OrdenNatural(), luego lee el orden dado en la string x y los ordena de
 * acuerdo a lo que lee. Es decir, si luego de OrdenNatural los vertices
 * quedaron ordenados como, por ejemplo, V[0],V[1],...,V[n-1], luego de
 * OrdenEspecifico el orden debe ser V[x[0]],V[x[1]],...,V[x[n-1]].
 * Ejemplo:
 * Supongamos que los vertices son 1, 10, 22, 5, 64, 7, 1001. OrdenNatural()
 * los ordena como 1,5,7,10,22,64,1001. (i.e. V[0]=1,V[1]=5, etc).
 * Supongamos que x=4,0,2,6,5,3,1. Entonces OrdenEspecifico(G,x) ordenará los
 * vértices como V[4],V[0],V[2],etc, es decir, 22,1,7,1001,64,10,5.
 */

list_t list_empty(void);
/*
 * Return a newly created, empty list.
 *
 * The caller must call list_destroy when done using the resulting list,
 * so the resources allocated by this call are freed.
 *
 * POST: the result is a valid list pointer, and list_length(result) is 0.
 */

list_t list_destroy(list_t list);
/*
 * Free the resources allocated for the given 'list', and set it to NULL.
 */

u32 list_length(list_t list);
/*
 * Return the amount of elements in the given 'list'.
 *
 * PRE: 'list' is a valid list pointer.
 */

u32 list_search(list_t list, u32 nombre);
/* Dado el nombre de un vértice, devuelve la posición de ese vértice en el grafo.
 */

u32 list_exists(list_t list, u32 nombre);
/* Dado el nombre de un vértice, devuelve true si el vértice existe en la lista de vecinos.
*/

u32 list_index(list_t list, u32 i);
/* Útil para recorrer todos los elementos de la lista.
 * El usuario es responsable por pasar un índice correcto.
 * Devuelve el index del vertice en la posicion i de la lista.
 */

list_t list_append(list_t list, u32 index, u32 nombre);
/*
 * Return the given 'list' with the ('index', 'data') added to the end.
 *
 * The given 'index' and 'data' are inserted in the list,
 * so they can not be destroyed by the caller.
 *
 * PRE: all 'list', 'index' and 'data' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'list'
 * plus one. The elements of the result are the same as the ones in 'list'
 * with the new pair ('index', 'data') added at the end of the list.
 *
 * In order words, using pseudo code, the POST is:
 * (list_length(list) + 1 == list_length(result)) and
 * (forall i: 0 <= i < list_length(list): list[i] == result[i]) and
 * (result[list_length(list)] == ('index', 'data')
 */

list_t list_remove(list_t list, u32 nombre);
/*
 * Return the given 'list' with the first (index, <data>) pair
 * occurrence removed.
 *
 * Please note that 'index' may not be in the list (thus an unchanged
 * list is returned).
 *
 * PRE: 'list' and 'index' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'list'
 * minus one if 'index' existed in 'list'. The elements of the result is
 * the same as the one in 'list' with the entry for 'index' removed.
 */

void list_dump(list_t list);

char* readline_from_stdin();
/* Lee de stdin MAX_LINE_LENGTH caracteres.
 * Devuelve string leido, sin '\n' final.
 * Usuario debe liberar memoria para dicho string.
 * Si en una línea hay más de MAX_LINE_LENGTH caracteres, devuelve NULL.
 */

par_t handle_p_line(char* line);
/* Util para leer línea de tipo "p edge n m", donde n y m son números.
 * Si línea respeta formato, devuelve n y m envueltos en struct par_t.
 * Caso contrario, devuelve NULL.
 * Usuario debe liberar memoria para par_t devuelto en caso de éxito.
 */

par_t handle_e_line(char* line);
/* Util para leer línea de tipo "e n m", donde n y m son números.
 * Si línea respeta formato, devuelve n y m envueltos en struct par_t.
 * Caso contrario, devuelve NULL.
 * Usuario debe liberar memoria para par_t devuelto en caso de éxito.
 */

u32 get_l(par_t par);
// Devuelve elelemento l del par.

u32 get_r(par_t par);
// Devuelve elelemento r del par.
