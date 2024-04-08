#include "../include/globals.h"

ALLEGRO_DISPLAY* chrus_display = NULL;
ALLEGRO_EVENT_QUEUE* chrus_drawing_queue = NULL;
ALLEGRO_EVENT_SOURCE chrus_drawing_event_source;
float audio_buffer[1024];

float* chrus_demo_get_audio_buffer() {
    return audio_buffer;
}

void chrus_demo_postprocess_callback(void* buf, unsigned int samples, void* data) {
    //printf("samples: %d\n", samples);
    float* fbuf = (float*)buf;

    memcpy(audio_buffer, fbuf, 1024*sizeof(float));
}

void chrus_join_drawing_thread() {
    ALLEGRO_EVENT e;
    e.user.type = CHRUS_EVENT_JOIN_DRAWING;

    printf("joining drawing thread now\n");

    al_emit_user_event(&chrus_drawing_event_source, &e, NULL);
}