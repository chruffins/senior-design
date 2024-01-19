#include "../include/scenemanager.h"

void chrus_scene_manager_init(chrus_scene_manager *ptr) {
    memset(ptr->scenes, 0, sizeof(chrus_scene*)*MAX_SCENES);
    memset(ptr->queues, 0, sizeof(ALLEGRO_EVENT_QUEUE*)*MAX_SCENES);
    ptr->mutex = al_create_mutex_recursive();
    ptr->top = -1;
}

chrus_scene* chrus_scene_manager_top(chrus_scene_manager *this) {

    return this->scenes[this->top];
}

chrus_scene* chrus_scene_manager_at(chrus_scene_manager *this, int16_t at) {
    if (at < 0 || at > this->top) return NULL;

    return this->scenes[at];
}

int chrus_scene_manager_add_scene(chrus_scene_manager *this, chrus_scene *new_scene) {
    al_lock_mutex(this->mutex);

    if (this->top == MAX_SCENES - 1) {
        al_unlock_mutex(this->mutex);
        return -1;
    }
    this->top++;
    this->queues[this->top] = al_create_event_queue(); // replace with chrus_create_scene_event_queue();
    this->scenes[this->top] = new_scene;

    al_unlock_mutex(this->mutex);
    return 0;
}

int chrus_scene_manager_pop_scene(chrus_scene_manager *this) {
    al_lock_mutex(this->mutex);

    printf("popping scene now\n");
    if (this->top == -1) return -1;

    chrus_scene_destroy(this->scenes[this->top]);
    al_destroy_event_queue(this->queues[this->top]);
    this->queues[this->top] = NULL;
    this->scenes[this->top] = NULL;
    this->top--;

    al_unlock_mutex(this->mutex);

    return 0;
}

int chrus_scene_manager_set_modal_scene(chrus_scene_manager *this, int16_t at) {
    al_lock_mutex(this->mutex);

    for (int16_t i = 0; i < this->top; i++) {
        al_pause_event_queue(this->queues[i], true);
    }

    al_pause_event_queue(this->queues[at], false);
    this->current_modal = at;

    al_unlock_mutex(this->mutex);
    return 0;
}

void chrus_scene_manager_reset_modal(chrus_scene_manager *this) {
    al_lock_mutex(this->mutex);

    for (int16_t i = 0; i < this->top; i++) {
        al_pause_event_queue(this->queues[i], true);
    }

    this->current_modal = -1;

    al_unlock_mutex(this->mutex);
}

void chrus_scene_manager_draw(chrus_scene_manager *this) {
    al_lock_mutex(this->mutex);

    for (int i = 0; i <= this->top; i++) {
        chrus_scene_draw(this->scenes[i]);
    }

    al_unlock_mutex(this->mutex);
}

void chrus_scene_manager_process_input(chrus_scene_manager *this, ALLEGRO_EVENT *event) {
    // TODO: remove this.
    //al_lock_mutex(this->mutex);

    if (this->current_modal < 0) {
        for (int i = 0; i < this->top; i++) {
            chrus_scene_process_input(this->scenes[i], event);
        }
    } else {
        chrus_scene_process_input(this->scenes[this->current_modal], event);
    }

    //al_unlock_mutex(this->mutex);
}

void chrus_scene_manager_destroy(chrus_scene_manager* this) {
    al_lock_mutex(this->mutex);

    int result = chrus_scene_manager_pop_scene(this);
    while (!result) {
        result = chrus_scene_manager_pop_scene(this);
    }

    al_unlock_mutex(this->mutex);
    al_destroy_mutex(this->mutex);
}