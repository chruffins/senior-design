/*
  _____ _                 _____  ______ _____ 
 / ____| |               |  __ \|_   _/ ____|
| |    | |__   __ _ _ __ | |__) | | | | (___  
| |    | '_ \ / _` | '_ \|  ___/  | |  \___ \ 
| |____| | | | (_| | | | | |      | |  ____) |
 \_____|_| |_|\__,_|_| |_|_|      |_| |_____/ 

I steal ASCII art too!
*/

#pragma once
#include<stddef.h>

typedef struct chrus_node_t chrus_node;
typedef struct chrus_node_vector_t chrus_node_vec;

typedef chrus_node* chrus_nodeptr;

struct chrus_node_t {
    chrus_node* parent;
    chrus_node_vec children;

    int type;
    void *data;
}; // chrus is pretty long but should be the prefix for all names... thanks C!

struct chrus_node_vector_t {
    chrus_node *data;
    size_t size;
    size_t capacity;
};

chrus_node chrus_node_create();
chrus_node_vec chrus_node_vec_create();


void chrus_node_reparent(chrus_node *parent, chrus_node *new_child);


