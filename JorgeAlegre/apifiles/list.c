#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "list.h"

struct _node_t {
    u32 nombre;
    u32 index;
    list_t next;
};


list_t list_empty() {
    list_t list = NULL;

    return (list);
}

list_t list_destroy(list_t list) {
    list_t aux = NULL;

    if (list != NULL) {
        //Destroy every node but first.
        while (list->next != NULL) {
            //While the second node exists.
            //Make aux point to first node.
            aux = list;
            //Make list point to second node.
            list = aux->next;
            //Clear first node.
            aux->next = NULL;
            free(aux);
            aux = NULL;
        }

        //Destroy first node.
        free(list);
        list = NULL;
    }

    assert(list == NULL);

    return (list);
}

u32 list_index(list_t list, u32 i) {
    list_t aux = list;
    u32 j = 0;

    while (j < i) {
        aux = aux->next;
        j++;
    }

    return (aux->index);
}

list_t list_append(list_t list, u32 index, u32 nombre) {
    list_t new = NULL;

    new = calloc(1, sizeof(struct _node_t));
    assert(new != NULL);
    new->next = list;
    new->index = index;
    new->nombre = nombre;

    return (new);
}

void list_dump(list_t list) {
    list_t aux = list;

    while (aux != NULL) {
        if (aux != NULL) {
            printf("Vertice %u, posicion %u\n", aux->nombre, aux->index);
        }
        aux = aux->next;
    }
}
