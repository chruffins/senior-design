#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<memory.h>

#include<allegro5/allegro.h>
#include "node.h"

// basically a top level node lol
struct chrus_scene_t {
    const char *name;
    chrus_node *current_camera;
    chrus_node_vec children;
} typedef chrus_scene;

chrus_scene *chrus_scene_create(const char *name);
void chrus_scene_destroy(chrus_scene *scene);

chrus_scene *chrus_scene_from_file(const char *filename);

void chrus_scene_process_input(chrus_scene* restrict this, ALLEGRO_EVENT *event);

void chrus_scene_draw(chrus_scene* restrict this);