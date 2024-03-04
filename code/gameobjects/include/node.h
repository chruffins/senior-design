/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Nodes act as a shell for the other game objects.
*/

#pragma once
#include<stddef.h>
#include<stdlib.h>
#include<memory.h>
#include<stdbool.h>

#include "node_types.h"
#include "camera.h"
#include "script.h"
#include "sound.h"
#include "sprite.h"

typedef struct chrus_node_t chrus_node;
typedef struct chrus_node_vector_t chrus_node_vec;

typedef chrus_node* chrus_nodeptr;

struct chrus_node_vector_t {
    chrus_node **data;
    size_t size;
    size_t capacity;
};

struct chrus_node_t {
    const char *name;
    chrus_node* parent;
    chrus_node_vec children;

    enum CHRUS_NODE_TYPES type;
    void *data;
}; // chrus is pretty long but should be the prefix for all names... thanks C!

chrus_node* chrus_node_create_uninit();
chrus_node* chrus_node_create_camera();
chrus_node* chrus_node_create_sprite();
chrus_node* chrus_node_create_audiostream();
chrus_node_vec chrus_node_vec_create();

void chrus_node_destroy(chrus_node *this);
//void chrus_node_reparent(chrus_node *parent, chrus_node *new_child);

void chrus_node_vec_destroy(chrus_node_vec* this);
bool chrus_node_vec_add_node(chrus_node_vec *this, chrus_node *new_child);