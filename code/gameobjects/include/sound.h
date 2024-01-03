#pragma once

#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

#include<stdbool.h>
#include<stdio.h>

typedef struct chrus_audiostream_t chrus_audiostream;
typedef struct chrus_sound_t chrus_sound;
//typedef struct chrus_audiostream_t chrus_audiostream_t;

struct chrus_sound_t {
    ALLEGRO_SAMPLE *sample;
    ALLEGRO_SAMPLE_INSTANCE *sound;
    float length;
    ALLEGRO_PLAYMODE playmode;
    float gain;
    float pan;
    float speed;
};

struct chrus_audiostream_t {
    ALLEGRO_AUDIO_STREAM *stream;
    float length;
    ALLEGRO_PLAYMODE playmode;
    float gain;
    float pan;
    float speed; 
};

chrus_sound* chrus_sound_create(const char* source);
void chrus_sound_load(chrus_sound* restrict this, const char *source);
void chrus_sound_play(chrus_sound* restrict this);
void chrus_sound_stop(chrus_sound* restrict this);
void chrus_sound_destroy(chrus_sound* restrict this);

chrus_audiostream* chrus_audiostream_create(const char *source);
void chrus_audiostream_load(chrus_audiostream* restrict this, const char *source);
void chrus_audiostream_play(chrus_audiostream* restrict this);
void chrus_audiostream_stop(chrus_audiostream* restrict this);
void chrus_audiostream_destroy(chrus_audiostream* restrict this);