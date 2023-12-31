#include "../include/scenemanager.h"

void chrus_scene_manager_init(chrus_scene_manager *ptr) {
    memset(ptr->scenes, 0, sizeof(chrus_scene*)*MAX_SCENES);
    ptr->top = -1;
}

chrus_scene *chrus_scene_manager_top(chrus_scene_manager *this) {
    return this->scenes[this->top];
}

chrus_scene *chrus_scene_manager_at(chrus_scene_manager *this, int16_t at) {
    if (at < 0 || at > this->top) return NULL;

    return this->scenes[at];
}

int chrus_scene_manager_add_scene(chrus_scene_manager *this, chrus_scene *new_scene) {
    if (this->top == 255) return -1;
    this->top++;
    this->scenes[this->top] = new_scene;
    return 0;
}

int chrus_scene_manager_pop_scene(chrus_scene_manager *this) {
    printf("popping scene now\n");
    if (this->top == -1) return -1;

    chrus_scene_destroy(this->scenes[this->top]);
    this->scenes[this->top] = NULL;
    this->top--;

    return 0;
}

int chrus_scene_manager_set_modal_scene(chrus_scene_manager *this, int16_t at) {
    this->current_modal = at;

    return 0;
}

void chrus_scene_manager_reset_modal(chrus_scene_manager *this) {
    this->current_modal = -1;
}

void chrus_scene_manager_draw(chrus_scene_manager *this) {
    for (int i = 0; i <= this->top; i++) {
        chrus_scene_draw(this->scenes[i]);
    }
}

void chrus_scene_manager_process_input(chrus_scene_manager *this, ALLEGRO_EVENT *event) {
    if (this->current_modal < 0) {
        for (int i = 0; i < this->top; i++) {
            chrus_scene_process_input(this->scenes[i], event);
        }
    } else {
        chrus_scene_process_input(this->scenes[this->current_modal], event);
    }
}

void chrus_scene_manager_destroy(chrus_scene_manager* this) {
    int result = chrus_scene_manager_pop_scene(this);
    while (!result) {
        result = chrus_scene_manager_pop_scene(this);
    }
}