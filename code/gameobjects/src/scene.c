#include "../include/scene.h"

chrus_scene *chrus_scene_create(const char *name) {
    chrus_scene *new_scene = malloc(sizeof(chrus_scene));
    new_scene->name = name;
    new_scene->children = chrus_node_vec_create();
    new_scene->lua_vm = luaL_newstate();
    luaL_openlibs(new_scene->lua_vm);

    new_scene->sprites_cache = malloc(sizeof(chrus_sprite*) * 100); // lmao we're gonna change this later

    return new_scene;
}

void chrus_scene_destroy(chrus_scene *scene) {
    for (int i = 0; i < scene->children.size; i++) {
        chrus_node_destroy(scene->children.data[i]);
    }

    free(scene);
}

chrus_scene *chrus_scene_from_file(const char *filename) {
    chrus_scene *new_scene = malloc(sizeof(chrus_scene));

    // TODO: design the storage of scenes

    return new_scene;
}

void chrus_scene_process_input(chrus_scene* restrict this, ALLEGRO_EVENT *event) {
    // TODO: invent listeners
}

void chrus_scene_draw(chrus_scene* restrict this) {
    for (int i = 0; i < 100; i++) {
        
    }
}

chrus_node* chrus_scene_add_node(chrus_scene* restrict this, void* parent, chrus_node *child) {
    if (parent == this) {
        chrus_node_vec_add_node(&this->children, child);
    } else {
        chrus_node *cnparent = (chrus_node*) parent;
        chrus_node_vec_add_node(&cnparent->children, child);
    }

    switch (child->type)
    {
    case CHRUS_NODE_SCRIPT:
        chrus_scene_run_script(this, child);
        break;
    case CHRUS_NODE_SPRITE:

        break;
    default:
        break;
    }

    return child;
}

void chrus_scene_run_script(chrus_scene* restrict this, chrus_node *script) {
    assert(script->type == CHRUS_NODE_SCRIPT);
    chrus_script* data = script->data;

    int result = luaL_loadfile(this->lua_vm, data->source_name) || lua_pcall(this->lua_vm, 0, 0, 0);
    if (result) {
        printf("%s\n", lua_tostring(this->lua_vm, -1));
    }
}