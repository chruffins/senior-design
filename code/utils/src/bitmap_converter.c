#include "../include/bitmap_converter.h"

static inline void chrus_emit_conversion_event(ALLEGRO_BITMAP* bmp) {
    ALLEGRO_EVENT converter_event;
    converter_event.user.type = 0; /* TODO: figure out a value for this */
    converter_event.user.data1 = bmp;

    al_emit_user_event(&chrus_drawing_event_source, &converter_event, NULL);
}


ALLEGRO_BITMAP* chrus_create_bitmap(int w, int h) {
    
}

ALLEGRO_BITMAP* chrus_load_bitmap(const char* filename) {

}