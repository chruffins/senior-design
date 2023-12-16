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

//static ALLEGRO_BITMAP *default_image;

static chrus_sprite* chrus_sprite_create_default();
chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_draw(chrus_sprite *this);