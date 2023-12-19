#pragma once

#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

#include<stdbool.h>
#include<stdio.h>

typedef struct chrus_sound_t chrus_sound;

struct chrus_sound_t {
    ALLEGRO_AUDIO_STREAM *stream;
    float length;
    ALLEGRO_PLAYMODE playmode;
    float gain;
    float pan;
    float speed; 
};

chrus_sound *chrus_sound_create(const char *source);

void chrus_sound_load(chrus_sound* restrict this, const char *source);
void chrus_sound_play(chrus_sound* restrict this);
void chrus_sound_stop(chrus_sound* restrict this);
void chrus_sound_free(chrus_sound* restrict this);