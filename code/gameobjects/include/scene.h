#pragma once
#include "node.h"

// basically a top level node lol
struct scene_t {
    char *name;
    chrus_node_vec children;
} typedef scene;

