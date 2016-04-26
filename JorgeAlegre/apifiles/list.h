#ifndef _LIST_H
#define _LIST_H

#include "typedefs.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct _node_t  *list_t;

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

#endif
