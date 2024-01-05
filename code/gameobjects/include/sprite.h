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

#include<stdlib.h>

typedef struct chrus_sprite_t chrus_sprite;

struct chrus_sprite_t {
    float x;
    float y;
    int width;
    int height;
    int flipping;
    float rotation;

    ALLEGRO_BITMAP *image_data;
};

//static ALLEGRO_BITMAP *default_image;

//chrus_sprite* chrus_sprite_create_default();
chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_draw(chrus_sprite *this, float dx, float dy);
//void chrus_sprite_destroy();

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);