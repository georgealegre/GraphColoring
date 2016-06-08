/* Matemática Discreta II
 * Proyecto, primera parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "u32queue.h"

struct _u32queue_t {
    u32 first;
    u32 last;
    u32* queue;
};

u32queue_t queue_empty() {
    u32queue_t queue = NULL;

    // Pedir memoria para estructura de cola e inicializar.
    queue = calloc(1, sizeof(struct _u32queue_t));
    assert(queue != NULL);
    queue->first = 0;
    queue->last = 0;

    return (queue);
}

u32queue_t queue_destroy(u32queue_t queue) {
    if (queue != NULL) {
        // Eliminar cola.
        if (queue->queue != NULL) {
            free(queue->queue);
            queue->queue = NULL;
        }

        // Eliminar estructura.
        free(queue);
        queue = NULL;
    }

    return (NULL);
}

bool queue_is_empty(u32queue_t queue) {
    if (queue != NULL && (queue->last - queue->first) != 0)
        return false;
    else
        return true;
}

u32 queue_first(u32queue_t queue) {
    return (queue->queue[queue->first]);
}

u32queue_t queue_enqueue(u32queue_t queue, u32 index) {
    // Pedir espacio para nuevo index.
    queue->queue = realloc(queue->queue, (queue->last + 1) * sizeof(u32));

    queue->queue[queue->last] = index;

    // Aumentar tamaño de queue.
    queue->last++;

    return (queue);
}

u32queue_t queue_dequeue(u32queue_t queue) {
    // Aumentar por 1 índice del primer elemento.
    queue->first++;

    return queue;
}
