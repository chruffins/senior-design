#include "../include/bitmap_converter.h"

static inline void chrus_emit_conversion_event(ALLEGRO_BITMAP* bmp) {
    ALLEGRO_EVENT converter_event;
    converter_event.user.type = CHRUS_EVENT_CONVERT_BITMAP; /* TODO: figure out a value for this */
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