#include "../include/node.h"

/*
struct chrus_node_t {
    chrus_node* parent;
    chrus_node_vec children;

    enum CHRUS_NODE_TYPES type;
    void *data;
}; // chrus is pretty long but should be the prefix for all names... thanks C!
*/

chrus_node *chrus_node_create_uninit() {
    chrus_node *new_node = malloc(sizeof(chrus_node));
    *new_node = (chrus_node){ "unused", NULL, chrus_node_vec_create(), CHRUS_NODE_UNINITIALIZED, NULL };
    return new_node;
}

chrus_node *chrus_node_create_camera() {
    printf("creating camera node\n");

    chrus_node *new_node = malloc(sizeof(chrus_node));
    *new_node = (chrus_node){ "camera", NULL, chrus_node_vec_create(), CHRUS_NODE_CAMERA, chrus_camera_create() };
    return new_node;
}

chrus_node* chrus_node_create_sprite() {
    printf("creating sprite node\n");

    chrus_node *new_node = malloc(sizeof(chrus_node));
    *new_node = (chrus_node){ "sprite", NULL, chrus_node_vec_create(), CHRUS_NODE_SPRITE, chrus_sprite_create(NULL) };
    return new_node;
}

chrus_node* chrus_node_create_audiostream() {
    printf("creating stream node\n");

    chrus_node *new_node = malloc(sizeof(chrus_node));
    *new_node = (chrus_node){ "audiostream", NULL, chrus_node_vec_create(), CHRUS_NODE_AUDIOSTREAM, chrus_audiostream_create(NULL) };
    return new_node;
}

chrus_node_vec chrus_node_vec_create() {
    return (chrus_node_vec){ malloc(sizeof(chrus_node*)), 0, 1 };
}

void chrus_node_destroy(chrus_node *this) {
    for (size_t i = 0; i < this->children.size; i++) {
        chrus_node_destroy(this->children.data[i]);
    }
    chrus_node_vec_destroy(&this->children);

    switch (this->type)
    {
    case CHRUS_NODE_SCRIPT:
        //free(this->data);
        break;
    case CHRUS_NODE_CAMERA:
        chrus_camera_destroy(this->data);
        break;
    case CHRUS_NODE_AUDIOSTREAM:
        chrus_audiostream_destroy(this->data);
        break;
    case CHRUS_NODE_PRIMITIVE:
        chrus_prim_destroy(this->data);
        break;
    default:
        free(this->data);
        break;
    }

    free(this);
}

/*
void chrus_node_reparent(chrus_node *parent, chrus_node *new_child) {
    // we should probably propagate child added up the chain...
}
*/

/* equivalent to emptying */
void chrus_node_vec_destroy(chrus_node_vec* this) {
    if (this->capacity == 0 || this->data == NULL) return;

    free(this->data);
    this->capacity = 0;
    this->size = 0;
}

bool chrus_node_vec_add_node(chrus_node_vec *this, chrus_node *new_child) {
    /* printf("added %s to node vec\n", new_child->name); */
    if (this->size + 1 >= this->capacity) {
        void *newdata = realloc(this->data, this->capacity * sizeof(chrus_node*) * 2);
        if (newdata == NULL) {
            return false;
        }
        this->capacity *= 2;
        this->data = newdata;
    }
    this->data[this->size] = new_child;
    this->size++;

    return true;
}