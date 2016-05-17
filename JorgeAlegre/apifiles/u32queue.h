#ifndef _U32QUEUE_H
#define _U32QUEUE_H

#include <stdio.h>
#include <stdbool.h>

#include "typedefs.h"

typedef struct _u32queue_t *u32queue_t;

u32queue_t queue_empty(void);
/*
 * Return a newly created, empty queue.
 *
 * The caller must call queue_destroy when done using the resulting queue,
 * so the resources allocated by this call are freed.
 *
 * POST: the result is a valid queue pointer, and queue_is_empty(result) is true.
 */

u32queue_t queue_destroy(u32queue_t queue);
/*
 * Free the resources allocated for the given 'queue', and set it to NULL.
 */

bool queue_is_empty(u32queue_t queue);
/*
 * Return the amount of elements in the given 'queue'.
 *
 * PRE: 'queue' is a valid queue pointer.
 */

u32 queue_first(u32queue_t queue);

u32queue_t queue_enqueue(u32queue_t queue, u32 index);
/*
 * Return the given 'queue' with the ('index', 'data') added to the end.
 *
 * The given 'index' and 'data' are inserted in the queue,
 * so they can not be destroyed by the caller.
 *
 * PRE: all 'queue', 'index' and 'data' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'queue'
 * plus one. The elements of the result are the same as the ones in 'queue'
 * with the new pair ('index', 'data') added at the end of the queue.
 *
 * In order words, using pseudo code, the POST is:
 * (queue_length(queue) + 1 == queue_length(result)) and
 * (forall i: 0 <= i < queue_length(queue): queue[i] == result[i]) and
 * (result[queue_length(queue)] == ('index', 'data')
 */

u32queue_t queue_dequeue(u32queue_t queue);
/*
 * Return the given 'queue' with the first (index, <data>) pair
 * occurrence removed.
 *
 * Please note that 'index' may not be in the queue (thus an unchanged
 * queue is returned).
 *
 * PRE: 'queue' and 'index' are valid instances.
 *
 * POST: the length of the result is the same as the length of 'queue'
 * minus one if 'index' existed in 'queue'. The elements of the result is
 * the same as the one in 'queue' with the entry for 'index' removed.
 */

#endif
