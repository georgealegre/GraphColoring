#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>
#include "typedefs.h"

#define RED 1
#define BLACK 2

typedef struct _rb_tree_t *rb_tree_t;

rb_tree_t rb_new();

rb_tree_t rb_insert(rb_tree_t tree, u32 key, u32 name);

u32 rb_search(rb_tree_t tree, u32 key);

bool rb_exists(rb_tree_t tree, u32 key);

rb_tree_t rb_destroy(rb_tree_t tree);

#endif
