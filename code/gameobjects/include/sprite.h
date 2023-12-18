#pragma once

#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>

#include<stdlib.h>

struct chrus_sprite_t {
    float x;
    float y;
    int width;
    int height;
    int flipping;
    float rotation;

    ALLEGRO_BITMAP *image_data;
} typedef chrus_sprite;

struct chrus_sprite_vec_t {
    chrus_sprite **data;
    size_t size;
    size_t capacity;
} typedef chrus_sprite_vec;

//static ALLEGRO_BITMAP *default_image;

//chrus_sprite* chrus_sprite_create_default();
chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_draw(chrus_sprite *this, float dx, float dy);

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);