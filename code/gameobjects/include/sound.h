/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Defines the sample and audiostream structs, which are used to
 * contain short-form and long-form audios, respectively.
*/
#pragma once

#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>

#include<stdbool.h>
#include<stdio.h>

#include "../../utils/include/allocator.h"

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

// void chrus_sound_set_length(chrus_sound* restrict this, float new);
void chrus_sound_set_playmode(chrus_sound* restrict this, ALLEGRO_PLAYMODE new);
void chrus_sound_set_gain(chrus_sound* restrict this, float new);
void chrus_sound_set_pan(chrus_sound* restrict this, float new);
void chrus_sound_set_speed(chrus_sound* restrict this, float new);

void chrus_audiostream_set_playmode(chrus_audiostream* restrict this, ALLEGRO_PLAYMODE new);
void chrus_audiostream_set_gain(chrus_audiostream* restrict this, float new);
void chrus_audiostream_set_pan(chrus_audiostream* restrict this, float new);
void chrus_audiostream_set_speed(chrus_audiostream* restrict this, float new);

float chrus_sound_get_length(chrus_sound* restrict this);
ALLEGRO_PLAYMODE chrus_sound_get_playmode(chrus_sound* restrict this);
float chrus_sound_get_gain(chrus_sound* restrict this);
float chrus_sound_get_pan(chrus_sound* restrict this);
float chrus_sound_get_speed(chrus_sound* restrict this);

float chrus_audiostream_get_length(chrus_audiostream* restrict this);
ALLEGRO_PLAYMODE chrus_audiostream_get_playmode(chrus_audiostream* restrict this);
float chrus_audiostream_get_gain(chrus_audiostream* restrict this);
float chrus_audiostream_get_pan(chrus_audiostream* restrict this);
float chrus_audiostream_get_speed(chrus_audiostream* restrict this);