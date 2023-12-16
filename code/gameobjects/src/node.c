#include "../include/node.h"

/*
struct chrus_node_t {
    chrus_node* parent;
    chrus_node_vec children;

    enum CHRUS_NODE_TYPES type;
    void *data;
}; // chrus is pretty long but should be the prefix for all names... thanks C!
*/

chrus_node chrus_node_create_uninit() {
    return (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_UNINITIALIZED, NULL };
}

chrus_node_vec chrus_node_vec_create() {
    return (chrus_node_vec){ malloc(sizeof(chrus_node)), 0, 1 };
}

void chrus_node_destroy(chrus_node *this) {
    switch (this->type)
    {
    case CHRUS_NODE_SCRIPT:
        /* code */
    default:
        free(this->data);
        break;
    }

}

void chrus_node_reparent(chrus_node *parent, chrus_node *new_child) {
    // we should probably propagate child added up the chain...
}

void chrus_node_vec_add_node(chrus_node_vec *this, chrus_node *new_child) {
    if (this->size + 1 == this->capacity) {
        void *newdata = realloc(this->data, this->capacity * sizeof(chrus_node*) * 2);
        if (newdata == NULL) {
            // TODO: some kind of way to tell the world that we fucked up
        }
        this->capacity *= 2;
        this->data = newdata;
    }
    this->data[this->size] = new_child;
    this->size++;
}