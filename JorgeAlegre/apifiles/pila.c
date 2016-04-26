#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "pila.h"

struct _nodo_t {
    u32 index;
    pila_t next;
};

pila_t stack_empty() {
    pila_t stack = NULL;

    return (stack);
}

pila_t stack_destroy(pila_t stack) {
    pila_t aux = NULL;

    if (stack != NULL) {
        //Destroy every node but first.
        while (stack->next != NULL) {
            //While the second node exists.
            //Make aux point to first node.
            aux = stack;
            //Make stack point to second node.
            stack = aux->next;
            //Clear first node.
            aux->next = NULL;
            free(aux);
            aux = NULL;
        }

        //Destroy first node.
        free(stack);
        stack = NULL;
    }

    assert(stack == NULL);

    return (stack);
}

u32 stack_size(pila_t stack) {
    u32 length = 0;
    pila_t aux = stack;

    if (aux != NULL) {
        while (aux != NULL) {
            aux = aux->next;
            length++;
        }
    }

    return (length);
}

u32 stack_first(pila_t stack) {
    u32 result = 0;

    if (stack != NULL) {
        result = stack->index;
    }

    return (result);
}

pila_t stack_push(pila_t stack, u32 index) {
    pila_t head = NULL;
    head = calloc(1, sizeof(struct _nodo_t));
    head->index = index;
    head->next = stack;

    return (head);
}

pila_t stack_pop(pila_t stack) {
    pila_t aux = stack;

    if (stack != NULL) {
        stack = stack->next;
        free(aux);
        aux = NULL;
    }

    return (stack);
}

void stack_dump(pila_t stack) {
    pila_t aux = stack;

    if (aux != NULL) {
        while (aux != NULL) {
            printf("%u\n", aux->index);
            aux = aux->next;
        }
    }
}
