/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This header stores information for representing text.
*/
#pragma once

#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>

typedef struct chrus_text_t chrus_text;

struct chrus_text_t {
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
    float x;
    float y;
    float max_width;
    float line_height;
    int flags;
    const char* text;
};

chrus_text* chrus_text_create();
void chrus_text_draw(chrus_text* restrict this, float dx, float dy);
void chrus_text_destroy(chrus_text* restrict this);

const char* chrus_text_get_text(chrus_text* restrict this);

void chrus_text_set_text(chrus_text* restrict this, const char* new_text);