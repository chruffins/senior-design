/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Scenes are managed through a stack. Additional functionality includes
 * "modality", i.e., a modal scene will be the only scene processing input.
 * Scenes hande input and are rendered from top to bottom.
*/

#pragma once
#include<inttypes.h>
#include<memory.h>
#include<stdlib.h>
#include<allegro5/allegro.h>

//#include "../../utils/include/rbtree.h"

#include "scene.h"

#define MAX_SCENES 64

typedef struct chrus_scene_manager_t chrus_scene_manager;

// totally not intended to be a pointer... i think.
// this will ALWAYS be on the stack, never the heap
struct chrus_scene_manager_t {
    // basically a stack
    chrus_scene* scenes[MAX_SCENES]; // if you manage to use 65 scenes seek help
    ALLEGRO_THREAD* threads[MAX_SCENES];
    ALLEGRO_MUTEX* mutex;

    int8_t top; // stack pointer basically
    int8_t current_modal;
    //chrus_rbtree 
};

// because scene managers are only allocated on the stack...
void chrus_scene_manager_init(chrus_scene_manager *ptr);

chrus_scene *chrus_scene_manager_top(chrus_scene_manager* restrict this);
chrus_scene *chrus_scene_manager_at(chrus_scene_manager* restrict this, int16_t at);

int chrus_scene_manager_add_scene(chrus_scene_manager* restrict this, chrus_scene *new_scene);
int chrus_scene_manager_pop_scene(chrus_scene_manager* restrict this);
int chrus_scene_manager_set_modal_scene(chrus_scene_manager* restrict this, int16_t at); // setting a scene to modal means that only this scene will process input
void chrus_scene_manager_reset_modal(chrus_scene_manager* restrict this); // this internally sets the current modal scene to -1

void chrus_scene_manager_draw(chrus_scene_manager* restrict this);
void chrus_scene_manager_process_input(chrus_scene_manager* restrict this, ALLEGRO_EVENT *event);

void chrus_scene_manager_destroy(chrus_scene_manager* restrict this);