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

u32 list_length(list_t list) {
    u32 length = 0;
    list_t aux = list;

    if (aux != NULL) {
        while (aux != NULL && aux->next != NULL) {
            aux = aux->next;
            length++;
        }
        length++;
    }

    return (length);
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

unsigned int list_search(list_t list, u32 nombre) {
    list_t aux = list;
    u32 result = 0;
    bool encontrado = false;

    while (aux != NULL && !encontrado) {
        if (nombre == aux->nombre) {
            result = aux->index;
            encontrado = true;
        } else {
            aux = aux->next;
        }
    }

    return (result);
}

u32 list_exists(list_t list, u32 nombre) {
    list_t aux = list;
    bool result = false;

    while (aux != NULL && !result) {
        if (nombre == aux->nombre) {
            result = true;
        } else {
            aux = aux->next;
        }
    }

    return (result);
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
    list_t aux = NULL;

    if (list == NULL) {
        list = calloc(1, sizeof(struct _node_t));
        assert(list != NULL);
        list->index = index;
        list->nombre = nombre;
        list->next = NULL;
    } else {
        aux = list;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = calloc(1, sizeof(struct _node_t));
        aux->next->next = NULL;
        aux->next->index = index;
        aux->next->nombre = nombre;
    }

    return (list);
}

list_t list_remove(list_t list, u32 nombre) {
    unsigned int length = list_length(list);
    unsigned int n = 1, i = 1;
    list_t aux = list;
    list_t aux1 = list;

    if (list != NULL) {
        while (n < length && nombre != aux->nombre) {
            aux = aux->next;
            n++;
        }
        if (n == length && nombre == aux->nombre) {
            //The last node is the one I want to delete.
            //I have to destroy and free the last node and set the previous one to null.
            free(aux);
            if (length == 1) {
                list = NULL;
                return (list);
            } else {
                n = 1;
                aux = list;
                while (n < (length-1)) {
                    aux = aux->next;
                    n++;
                }
                aux->next = NULL;
                aux = NULL;
            }
        } else if (n == 1 && length > 1) {
            //List has more then one entry but I wish to delete the first one.
            list = aux->next;
            aux->next = NULL;
            free(aux);
            aux = NULL;
        } else if (nombre == aux->nombre) {
            //Node I want to remove is in middle of list.
            while (i < (n-1)) {
                aux1 = aux1->next;
                i++;
            }
            aux1->next = aux->next;
            free(aux);
            aux = NULL;
            aux1 = NULL;
        }
    }

    return (list);
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
