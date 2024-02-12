/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This header defines the sprite, the basic struct used to represent
 * two-dimensional images.
*/
#pragma once

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>

#include "../../utils/include/allocator.h"

#include<stdio.h>
#include<stdlib.h>

typedef struct chrus_sprite_t chrus_sprite;
typedef struct chrus_animation_t chrus_animation;
typedef struct chrus_animation_frame_t chrus_animation_frame;

struct chrus_sprite_t {
    const char* source; // path

    float x;
    float y;
    int width;
    int height;
    int flipping;
    float sx;
    float sy;
    float rotation;
    bool visible;
    
    ALLEGRO_BITMAP* image_data;
};

struct chrus_animation_t {
    int current_frame;
    int num_frames;
    int frame_counter;
    chrus_animation_frame* frames;
};

struct chrus_animation_frame_t {
    int frame_duration;
    ALLEGRO_BITMAP* frame_data;
};

//static ALLEGRO_BITMAP *default_image;

//chrus_sprite* chrus_sprite_create_default();
chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_load(chrus_sprite* restrict this, const char *source);
void chrus_sprite_draw(chrus_sprite* restrict this, float dx, float dy);
//void chrus_sprite_destroy();

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);

float chrus_sprite_get_x(chrus_sprite* restrict this);
float chrus_sprite_get_y(chrus_sprite* restrict this);
int chrus_sprite_get_width(chrus_sprite* restrict this);
int chrus_sprite_get_height(chrus_sprite* restrict this);
float chrus_sprite_get_sx(chrus_sprite* restrict this);
float chrus_sprite_get_sy(chrus_sprite* restrict this);
int chrus_sprite_get_flipping(chrus_sprite* restrict this);
float chrus_sprite_get_rotation(chrus_sprite* restrict this);
bool chrus_sprite_get_visible(chrus_sprite* restrict this);

void chrus_sprite_set_x(chrus_sprite* restrict this, float new);
void chrus_sprite_set_y(chrus_sprite* restrict this, float new);
void chrus_sprite_set_sx(chrus_sprite* restrict this, float new);
void chrus_sprite_set_sy(chrus_sprite* restrict this, float new);
void chrus_sprite_set_flipping(chrus_sprite* restrict this, int new);
void chrus_sprite_set_rotation(chrus_sprite* restrict this, float new);
void chrus_sprite_set_visible(chrus_sprite* restrict this, bool new);