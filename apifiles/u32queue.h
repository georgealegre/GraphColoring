/* Matemática Discreta II
 * Proyecto, primera parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#ifndef _U32QUEUE_H
#define _U32QUEUE_H

#include <stdio.h>
#include <stdbool.h>

#include "typedefs.h"

typedef struct _u32queue_t *u32queue_t;

u32queue_t queue_empty(void);

u32queue_t queue_destroy(u32queue_t queue);

bool queue_is_empty(u32queue_t queue);

u32 queue_first(u32queue_t queue);

u32queue_t queue_enqueue(u32queue_t queue, u32 index);

u32queue_t queue_dequeue(u32queue_t queue);

#endif
