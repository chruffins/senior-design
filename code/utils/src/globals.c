#include "../include/globals.h"

ALLEGRO_DISPLAY* chrus_display = NULL;
ALLEGRO_EVENT_QUEUE* chrus_drawing_queue = NULL;
ALLEGRO_EVENT_SOURCE chrus_drawing_event_source;

void chrus_join_drawing_thread() {
    ALLEGRO_EVENT e;
    e.user.type = CHRUS_EVENT_JOIN_DRAWING;

    printf("joining drawing thread now\n");

    al_emit_user_event(&chrus_drawing_event_source, &e, NULL);
}