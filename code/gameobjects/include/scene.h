/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Acts as the top-level node in an entity hierarchy. Each scene gets
 * their own Lua virtual machine to execute scripts.
*/

#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<memory.h>

#include<luajit-2.1/lua.h>
#include<luajit-2.1/lualib.h>
#include<luajit-2.1/lauxlib.h>

#include<allegro5/allegro.h>
#include "node.h"
#include "script.h"
#include "sprite.h"
#include "../../utils/include/vector.h"

typedef struct chrus_scene_t chrus_scene;

// basically a top level node lol
struct chrus_scene_t {
    const char *name;
    chrus_node *current_camera;
    lua_State *lua_vm; // we run the scripts in this environemnt
    chrus_node_vec children;
    chrus_vector sprites_cache; // component we use for direct access to sprite pointers
};

chrus_scene *chrus_scene_create(const char *name);
void chrus_scene_destroy(chrus_scene *scene);
chrus_scene *chrus_scene_from_file(const char *filename);

void chrus_scene_init_lua_vm(chrus_scene* restrict this);

void chrus_scene_process_input(chrus_scene* restrict this, ALLEGRO_EVENT *event);
void chrus_scene_draw(chrus_scene* restrict this);
chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child);

void chrus_scene_add_spritecache(chrus_scene* restrict this, chrus_sprite *sprite);
void chrus_scene_run_script(chrus_scene* restrict this, chrus_node *script);