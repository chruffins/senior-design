/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Not a game object because these should be handled internally.
 * Fonts are stored with their sizes as an unsorted dynamic array...
*/
#pragma once

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>

#include<string.h>
#include<stdio.h>

typedef struct chrus_font_t chrus_font;

/* i sure wish types could be file scope only */
struct chrus_fontsize_elem_t {
    int size;
    ALLEGRO_FONT* font;
};

struct chrus_font_t {
    const char* source;

    struct chrus_fontsize_elem_t* sizes;
    int capacity;
    int size;
};

chrus_font* chrus_font_create(const char* source);
void chrus_font_destroy(chrus_font* restrict this);

ALLEGRO_FONT* chrus_font_append_size(chrus_font* restrict this, int size);
ALLEGRO_FONT* chrus_font_get_size(chrus_font* restrict this, int size);