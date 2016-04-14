#ifndef _VERTICEST_H
#define _VERTICEST_H

#include "NimheP.h"

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

#endif
