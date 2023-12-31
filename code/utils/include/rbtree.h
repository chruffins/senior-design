/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This is my customized implementation of the red-black tree
 * data structure, which is used to map asset paths to their respective
 * pointers in memory.
*/
#pragma once

#include<stdlib.h>
#include<stdbool.h>

typedef struct chrus_rbnode_t chrus_rbnode;
typedef struct chrus_rbtree_t chrus_rbtree;

typedef int (*chrus_rbtree_comparator)(const void *, const void *);
typedef void (*chrus_rbtree_destructor)(void *);
typedef void *(*chrus_rbtree_inserter)(const void *);

struct chrus_rbnode_t {
    chrus_rbnode *parent;
    chrus_rbnode *left;
    chrus_rbnode *right;
    void *key;
    void *value;
    bool red; // 0 is black, 1 is red
};

struct chrus_rbtree_t {
    chrus_rbtree_comparator comparator;
    chrus_rbtree_destructor destructor;
    chrus_rbtree_inserter inserter; // i use this function to get the value based on the key

    chrus_rbnode *root;
};

chrus_rbtree *chrus_rbtree_create(chrus_rbtree_comparator, chrus_rbtree_destructor, chrus_rbtree_inserter);
void chrus_rbtree_destroy(chrus_rbtree *);

chrus_rbnode *chrus_rbtree_find(chrus_rbtree *, const void *key);
chrus_rbnode *chrus_rbtree_insert(chrus_rbtree *, const void *key);

int chrus_rbtree_delete(chrus_rbtree *, void *key);

int chrus_rbnode_child_direction(chrus_rbnode *parent, chrus_rbnode *child);
chrus_rbnode *chrus_rbnode_rotate(chrus_rbnode *current, bool right);

chrus_rbnode *chrus_rbtree_successor(chrus_rbtree *, chrus_rbnode *);