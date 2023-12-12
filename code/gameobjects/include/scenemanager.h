#pragma once
#include<inttypes.h>
#include<memory.h>
#include<stdlib.h>
#include "scene.h"

#define MAX_SCENES 256

// totally not intended to be a pointer... i think.
struct chrus_scene_manager_t {
    /* data */
    // basically a stack
    scene *scenes[MAX_SCENES]; // if you use 257 scenes i hope you die

    int16_t top; // don't worry about this too hard
} typedef chrus_scene_manager;

chrus_scene_manager* chrus_scene_manager_create();

scene *chrus_scene_manager_top();
scene *chrus_scene_manager_at(chrus_scene_manager*, uint8_t at);

int chrus_scene_manager_add_scene(chrus_scene_manager*, scene *new_scene);
int chrus_scene_manager_remove_scene(chrus_scene_manager*, scene *new_scene);

void chrus_scene_manager_destroy(chrus_scene_manager*);