#include "../include/bitmap_converter.h"

static inline void chrus_emit_conversion_event(ALLEGRO_BITMAP* bmp) {
    ALLEGRO_EVENT converter_event;
    converter_event.user.type = CHRUS_EVENT_CONVERT_BITMAP;
    converter_event.user.data1 = (intptr_t)bmp;

    al_emit_user_event(&chrus_drawing_event_source, &converter_event, NULL);
}

ALLEGRO_BITMAP* chrus_create_bitmap(int w, int h) {
    ALLEGRO_BITMAP* new_bitmap = al_create_bitmap(w, h);
    if (!new_bitmap) return NULL;

    chrus_emit_conversion_event(new_bitmap);
    return new_bitmap;
}

ALLEGRO_BITMAP* chrus_load_bitmap(const char* filename) {
    ALLEGRO_BITMAP* new_bitmap = al_load_bitmap(filename);
    if (!new_bitmap) return NULL;

    chrus_emit_conversion_event(new_bitmap);
    return new_bitmap;
}

ALLEGRO_SHADER* chrus_shader_create() {
    /* writing synchronous code l_l */
    ALLEGRO_SHADER** ptr_to_shader = malloc(sizeof(void*));
    ALLEGRO_SHADER* created_shader;
    ALLEGRO_MUTEX* shader_mutex = al_create_mutex();
    ALLEGRO_COND* shader_created = al_create_cond();
    ALLEGRO_EVENT converter_event;
    converter_event.user.type = CHRUS_EVENT_CREATE_SHADER;
    converter_event.user.data1 = (intptr_t)ptr_to_shader;
    converter_event.user.data2 = (intptr_t)shader_created;

    /* apparently you have to create and lock a mutex before waiting on a cond */
    al_lock_mutex(shader_mutex);
    al_emit_user_event(&chrus_drawing_event_source, &converter_event, NULL);
    al_wait_cond(shader_created, shader_mutex);
    al_unlock_mutex(shader_mutex);

    created_shader = *ptr_to_shader;

    free(ptr_to_shader);
    al_destroy_mutex(shader_mutex);
    al_destroy_cond(shader_created);

    return created_shader;
}

void chrus_shader_attach_source_file(ALLEGRO_SHADER* restrict shader, ALLEGRO_SHADER_TYPE type, const char* filename) {
    ALLEGRO_EVENT e;
    e.user.type = CHRUS_EVENT_ATTACH_SOURCEFILE_SHADER;
    e.user.data1 = (intptr_t)shader;
    e.user.data2 = (intptr_t)type;
    e.user.data3 = (intptr_t)filename;

    al_emit_user_event(&chrus_drawing_event_source, &e, NULL);
}

void chrus_shader_attach_source(ALLEGRO_SHADER* restrict shader, ALLEGRO_SHADER_TYPE type, const char* source) {
    ALLEGRO_EVENT e;
    e.user.type = CHRUS_EVENT_ATTACH_SOURCE_SHADER;
    e.user.data1 = (intptr_t)shader;
    e.user.data2 = (intptr_t)type;
    e.user.data3 = (intptr_t)source;

    al_emit_user_event(&chrus_drawing_event_source, &e, NULL);
}

void chrus_shader_build(ALLEGRO_SHADER* shader) {
    ALLEGRO_EVENT e;
    e.user.type = CHRUS_EVENT_BUILD_SHADER;
    e.user.data1 = (intptr_t)shader;

    al_emit_user_event(&chrus_drawing_event_source, &e, NULL);
}