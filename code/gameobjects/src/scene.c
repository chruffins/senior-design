#include "../include/scene.h"

chrus_scene *chrus_scene_create(const char *name) {
    printf("creating scene %s\n", name);

    chrus_scene *new_scene = malloc(sizeof(chrus_scene));
    
    al_init_user_event_source(&new_scene->event_source);
    new_scene->name = name;
    new_scene->children = chrus_node_vec_create();
    new_scene->lua_vm = luaL_newstate();
    /* we need to defer creating the camera to the thread handle*/
    new_scene->current_camera = NULL;
    new_scene->event_queue = al_create_event_queue();
    new_scene->tick_timer = al_create_timer(1.0 / 30.0);

    al_register_event_source(new_scene->event_queue, (ALLEGRO_EVENT_SOURCE*)new_scene);
    al_register_event_source(new_scene->event_queue, al_get_timer_event_source(new_scene->tick_timer));

    //chrus_scene_init_lua_vm(new_scene);

    new_scene->sprites_cache = chrus_vector_create();
    return new_scene;
}

/* needs to be called after the thread is joined */
void chrus_scene_destroy(chrus_scene *scene) {
    printf("destroying %lu nodes\n", scene->children.size);
    for (size_t i = 0; i < scene->children.size; i++) {
        /* printf("destroying node %s now\n", scene->children.data[i]->name); */
        chrus_node_destroy(scene->children.data[i]);
    }

    al_destroy_user_event_source(&scene->event_source);
    al_destroy_timer(scene->tick_timer);
    al_destroy_event_queue(scene->event_queue);

    lua_close(scene->lua_vm);

    free(scene);
}

chrus_scene *chrus_scene_from_file(const char *filename) {
    chrus_scene *new_scene = malloc(sizeof(chrus_scene));

    // TODO: design the storage of scenes

    return new_scene;
}

/* oh fuck yeah */
void* chrus_scene_thread_handler(ALLEGRO_THREAD* restrict this, void* args) {
    chrus_scene* scene = (chrus_scene*) args;
    
    printf("scene thread %p: starting now\n", this);

    /* we need this to create bitmaps in a thread successfully */
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    if (!scene->current_camera) {
        scene->current_camera = chrus_node_create_camera();
        chrus_camera_init(scene->current_camera->data);
        chrus_scene_add_node(scene, scene, scene->current_camera);
    } else {
        chrus_camera_init(scene->current_camera->data);
    }

    luaL_openlibs(scene->lua_vm);

    int result = luaL_dofile(scene->lua_vm, "data/chrus_ffi.lua");
    if (result) printf("%s\n", lua_tostring(scene->lua_vm, -1));

    lua_pushlightuserdata(scene->lua_vm, scene);
    lua_setglobal(scene->lua_vm, "scene");

    lua_pushlightuserdata(scene->lua_vm, this);
    lua_setglobal(scene->lua_vm, "thread");

    printf("scene thread %p: lua VM initialized\n", this);

    /* the thread now runs the event loop. the loop will end when join_thread is called */
    result = luaL_dofile(scene->lua_vm, "data/events.lua");
    if (result) {
        printf("scene thread %p: really big fuck up\n", this);
        printf("scene thread %p: %s\n", this, lua_tostring(scene->lua_vm, -1));
    }

    return NULL;
}

void chrus_scene_init_lua_vm(chrus_scene* restrict this) {
    lua_State *lua_vm = this->lua_vm;
    //const char *init = "ffi = require(\"ffi\")";
    //const char *scene_cast = "";

    luaL_openlibs(lua_vm);
    //luaL_dostring(lua_vm, init);
    int result = luaL_dofile(lua_vm, "data/chrus_ffi.lua");
    if (result) printf("%s\n", lua_tostring(lua_vm, -1));

    lua_pushlightuserdata(lua_vm, this);
    lua_setglobal(lua_vm, "scene");
}

void chrus_scene_process_input(chrus_scene* this, ALLEGRO_EVENT *event) {
    // TODO: invent listeners
}

void chrus_scene_draw(chrus_scene* restrict this) {
    chrus_camera* restrict current_camera = (chrus_camera*)this->current_camera->data;
    ALLEGRO_DISPLAY* current_display = al_get_current_display();

    //al_use_transform(&current_camera->_scaler);
    if (!current_camera->_buffer) return;
    al_set_target_bitmap(current_camera->_buffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    //al_hold_bitmap_drawing(true);

    for (int i = 0; i < this->children.size; i++) {
        chrus_node* node = this->children.data[i];
        switch (node->type)
        {
        case CHRUS_NODE_SPRITE:
            chrus_sprite_draw(node->data, -current_camera->viewport_x, -current_camera->viewport_y);
            break;
        case CHRUS_NODE_TEXT:
            chrus_text_draw(node->data, -current_camera->viewport_x, -current_camera->viewport_y);
            break;
        default:
            break;
        }
    }
    /*
    for (int i = 0; i < this->sprites_cache.size; i++) {
        chrus_sprite* restrict s = (chrus_sprite*)this->sprites_cache.data[i];
        if ((s->x + s->width > current_camera->viewport_x && s->x < current_camera->viewport_x + current_camera->viewport_width) && 
                (s->y + s->height > current_camera->viewport_y && s->y < current_camera->viewport_y + current_camera->viewport_height)) {
            chrus_sprite_draw(s, -current_camera->viewport_x, -current_camera->viewport_y);
        }
    }
    */

    //al_hold_bitmap_drawing(false);

    al_set_target_backbuffer(current_display);
    al_use_transform(&current_camera->_scaler);
    al_draw_bitmap(current_camera->_buffer, current_camera->screen_x, current_camera->screen_y, 0);

    // set target backbuffer is called by scenemanager
}

chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child) {
    if (parent == this) {
        chrus_node_vec_add_node(&this->children, child);
    } else {
        chrus_node *cnparent = (chrus_node*) parent;
        chrus_node_vec_add_node(&cnparent->children, child);
    }
    child->parent = parent;

    switch (child->type)
    {
    case CHRUS_NODE_SCRIPT:
        chrus_scene_queue_script(this, child);
        break;
    case CHRUS_NODE_SPRITE:
        chrus_vector_append(&this->sprites_cache, child->data);
        break;
    default:
        break;
    }

    return child;
}

chrus_node* chrus_scene_replace_rbnode(chrus_scene* this, void* parent, void** vec_ptr) {
    chrus_node* child = ((chrus_rbnode*)*(vec_ptr))->value;

    if (!child) return NULL; /* this really shouldn't happen... */

    *vec_ptr = child; /* replaces the pointer in the vector with the pointer to the child */
    child->parent = parent;

    switch (child->type)
    {
    case CHRUS_NODE_SCRIPT:
        chrus_scene_queue_script(this, child);
        break;
    case CHRUS_NODE_SPRITE:
        chrus_vector_append(&this->sprites_cache, child->data);
        break;
    case CHRUS_NODE_TEXT:
        //((chrus_text*)child->data)->font = chrus_text
        break;
    default:
        break;
    }

    return child;
}

void chrus_scene_queue_script(chrus_scene* this, chrus_node* script) {
    assert(script->type == CHRUS_NODE_SCRIPT);

    /* the new event only needs these fields filled out */
    ALLEGRO_EVENT new_event;
    new_event.user.type = CHRUS_EVENT_LOAD_SCRIPT;
    new_event.user.data1 = (intptr_t)script;

    al_emit_user_event((ALLEGRO_EVENT_SOURCE*)this, &new_event, NULL);
}