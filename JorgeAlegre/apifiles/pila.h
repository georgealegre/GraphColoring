#ifndef _PILA_H
#define _PILA_H

#include <stdio.h>
#include <stdbool.h>

#include "typedefs.h"

typedef struct _nodo_t  *pila_t;

pila_t stack_empty(void);
/*
 * Return a newly created, empty stack.
 *
 * The caller must call stack_destroy when done using the resulting stack,
 * so the resources allocated by this call are freed.
 *
 * POST: the result is a valid stack pointer, and stack_length(result) is 0.
 */

pila_t stack_destroy(pila_t stack);
/*
 * Free the resources allocated for the given 'stack', and set it to NULL.
 */

u32 stack_size(pila_t stack);
/*
 * Return the amount of elements in the given 'stack'.
 *
 * PRE: 'stack' is a valid stack pointer.
 */

u32 stack_first(pila_t stack);

pila_t stack_push(pila_t stack, u32 index);
/*
 * Return the given 'stack' with the ('index', 'data') added to the end.
 *
 * The given 'index' and 'data' are inserted in the stack,
 * so they can not be destroyed by the caller.
 *
 * PRE: all 'stack', 'index' and 'data' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'stack'
 * plus one. The elements of the result are the same as the ones in 'stack'
 * with the new pair ('index', 'data') added at the end of the stack.
 *
 * In order words, using pseudo code, the POST is:
 * (stack_length(stack) + 1 == stack_length(result)) and
 * (forall i: 0 <= i < stack_length(stack): stack[i] == result[i]) and
 * (result[stack_length(stack)] == ('index', 'data')
 */

pila_t stack_pop(pila_t stack);
/*
 * Return the given 'stack' with the first (index, <data>) pair
 * occurrence removed.
 *
 * Please note that 'index' may not be in the stack (thus an unchanged
 * stack is returned).
 *
 * PRE: 'stack' and 'index' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'stack'
 * minus one if 'index' existed in 'stack'. The elements of the result is
 * the same as the one in 'stack' with the entry for 'index' removed.
 */

void stack_dump(pila_t stack);

#endif
