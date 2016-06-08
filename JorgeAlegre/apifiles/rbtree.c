/* Matemática Discreta II
 * Proyecto, primera parte.
 * Autor: Alegre, Jorge Facundo <facu.alegre@gmail.com>
 */

#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#include <stdio.h>

#include "typedefs.h"
#include "rbtree.h"

struct _rb_tree_t {
    int color;
    rb_tree_t parent;
    rb_tree_t left;
    rb_tree_t right;
    u32 key;
    u32 value;
    bool nil;
};

rb_tree_t left_rotate(rb_tree_t tree, rb_tree_t x) {
    rb_tree_t y = NULL;

    y = x->right;               // Set y.
    x->right = y->left;         // Turn y's left subtree into x's right subtree.
    if (!y->left->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;      // Link x's parent to y.
    if (x->parent->nil) {
        tree = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;                // Put x on y's left.
    x->parent = y;

    return (tree);
}

rb_tree_t right_rotate(rb_tree_t tree, rb_tree_t x) {
    rb_tree_t y = NULL;

    y = x->left;                // Set y.
    x->left = y->right;         // Turn y's right subtree into x's left subtree.
    if (!y->right->nil) {
        y->right->parent = x;
    }
    y->parent = x->parent;      // Link x's parent to y.
    if (x->parent->nil) {
        tree = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;                // Put x on y's right.
    x->parent = y;

    return (tree);
}

rb_tree_t rb_new() {
    rb_tree_t result = NULL;

    result = calloc(1, sizeof(struct _rb_tree_t));
    assert(result != NULL);

    result->parent = result;
    result->nil = true;
    result->color = BLACK;

    return (result);
}

rb_tree_t rb_insert_fixup(rb_tree_t tree, rb_tree_t new) {
    rb_tree_t y = NULL;

    while (new->parent->color == RED) {
        if (new->parent == new->parent->parent->left) {
            y = new->parent->parent->right;
            if (y->color == RED) {
                new->parent->color = BLACK;
                y->color = BLACK;
                new->parent->parent->color = RED;
                new = new->parent->parent;
            } else {
                if (new == new->parent->right) {
                    new = new->parent;
                    tree = left_rotate(tree, new);
                }
                new->parent->color = BLACK;
                new->parent->parent->color = RED;
                tree = right_rotate(tree, new->parent->parent);
            }
        } else {
            y = new->parent->parent->left;
            if (y->color == RED) {
                new->parent->color = BLACK;
                y->color = BLACK;
                new->parent->parent->color = RED;
                new = new->parent->parent;
            } else {
                if (new == new->parent->left) {
                    new = new->parent;
                    tree = right_rotate(tree, new);
                }
                new->parent->color = BLACK;
                new->parent->parent->color = RED;
                tree = left_rotate(tree, new->parent->parent);
            }
        }
    }

    tree->color = BLACK;

    return (tree);
}

rb_tree_t rb_insert(rb_tree_t tree, u32 key, u32 value) {
    rb_tree_t y = tree->parent;
    rb_tree_t aux = tree;
    rb_tree_t new = rb_new();

    new->key = key;
    new->value = value;
    new->color = RED;
    new->nil = false;

    while (!aux->nil) {
        y = aux;
        if (new->key < aux->key) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }

    new->parent = y;
    if (y->nil) {
        tree = new;
        tree->left = y;
        tree->right = y;
    } else {
        if (new->key < y->key) {
            y->left = new;
        } else {
            y->right = new;
        }
        new->left = tree->parent;
        new->right = tree->parent;
    }

    tree = rb_insert_fixup(tree, new);

    return (tree);
}

bool rb_exists(rb_tree_t tree, u32 key) {
    if (tree != NULL && !tree->nil) {
        //Check if index given is equal to index from root.
        if (tree->key == key) {
            return true;
        } else if (key < tree->key && !tree->left->nil) {
            return rb_exists(tree->left, key);
        } else if (!tree->right->nil) {
            return rb_exists(tree->right, key);
        } else {
            return false;
        }
    } else {
        return false;
    }
}

u32 rb_search(rb_tree_t tree, u32 key) {
    if (tree != NULL && !tree->nil) {
        //Check if index given is equal to index from root.
        if (tree->key == key) {
            return tree->value;
        } else if (key < tree->key && !tree->left->nil) {
            return rb_search(tree->left, key);
        } else if (!tree->right->nil) {
            return rb_search(tree->right, key);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

rb_tree_t rb_destroy_rec(rb_tree_t tree) {
    rb_tree_t left = NULL;
    rb_tree_t right = NULL;

    if (tree != NULL && !tree->nil) {
        left = tree->left;
        tree->left = NULL;
        right = tree->right;
        tree->right = NULL;

        tree->parent = NULL;
        free(tree);
        tree = NULL;

        left = rb_destroy_rec(left);
        right = rb_destroy_rec(right);
    }

    return NULL;
}

rb_tree_t rb_destroy(rb_tree_t tree) {
    rb_tree_t result = NULL;

    if (tree != NULL && !tree->nil && tree->parent->nil) {
        result = tree->parent;
        result->left = NULL;
        result->right = NULL;

        tree = rb_destroy_rec(tree);

        free(result);
        result = NULL;
    } else if (tree != NULL && tree->nil) {
        tree->parent = NULL;
        tree->left = NULL;
        tree->right = NULL;
        free(tree);
        return NULL;
    }

    return (result);
}
