#ifndef _NIMHEP_H
#define _NIMHEP_H

#include <stdint.h>

#include "VerticeSt.h"

typedef u32 uint32_t;

typedef struct NimheSt *NimheP;

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

VerticeSt IesimoVerticeEnElOrden(NimheP G,u32 i);
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

#endif
