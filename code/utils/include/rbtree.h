#pragma once

#include<stdlib.h>
#include<stdbool.h>

typedef struct chrus_rbnode_t chrus_rbnode;
typedef struct chrus_rbtree_t chrus_rbtree;

struct chrus_rbnode_t {
    chrus_rbnode *parent;
    chrus_rbnode *left;
    chrus_rbnode *right;
    void *data;
    bool red; // 0 is black, 1 is red
};

struct chrus_rbtree_t {
    int (*comparator)(const void *, const void *);
    void (*destructor)(void *);

    chrus_rbnode *root;
};

chrus_rbtree *chrus_rbtree_create(int (*comparator)(const void *, const void *), void (*destructor)(void *));
void chrus_rbtree_destroy(chrus_rbtree *);

chrus_rbnode *chrus_rbtree_find(chrus_rbtree *, void *data);
chrus_rbnode *chrus_rbtree_insert(chrus_rbtree *, void *data);

// returns the key if found...
void *chrus_rbtree_delete(chrus_rbtree *, void *key);

int chrus_rbnode_child_direction(chrus_rbnode *parent, chrus_rbnode *child);
chrus_rbnode *chrus_rbnode_rotate(chrus_rbnode *current, bool right);


chrus_rbnode *chrus_rbtree_successor(chrus_rbtree *, chrus_rbnode *);