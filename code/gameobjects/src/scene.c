#include "../include/scene.h"

chrus_scene *chrus_scene_create(const char *name) {
    chrus_scene *new_scene = malloc(sizeof(chrus_scene));
    new_scene->name = new_scene;
    memset(&new_scene->children, 0, sizeof(chrus_node_vec));

    return new_scene;
}