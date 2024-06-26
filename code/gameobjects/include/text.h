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

#include<stdbool.h>
#include<stdio.h>

#include "../../utils/include/allocator.h"
#include "../../utils/include/font.h"

typedef struct chrus_text_t chrus_text;

/* TODO: refactor to use chrus_font */
struct chrus_text_t {
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
    float x;
    float y;
    float max_width;
    float line_height;
    int flags;
    int _width;
    int _layer;
    bool visible;
    const char* text;
};

bool chrus_load_default_font();

chrus_text* chrus_text_create();
void chrus_text_draw(chrus_text* restrict this, float dx, float dy);
void chrus_text_destroy(chrus_text* restrict this);

ALLEGRO_COLOR chrus_text_get_color(chrus_text* restrict this);
float chrus_text_get_x(chrus_text* restrict this);
float chrus_text_get_y(chrus_text* restrict this);
float chrus_text_get_max_width(chrus_text* restrict this);
float chrus_text_get_line_height(chrus_text* restrict this);
int chrus_text_get_flags(chrus_text* restrict this);
const char* chrus_text_get_text(chrus_text* restrict this);
int chrus_text_get_layer(chrus_text* restrict this);
int chrus_text_get_width(chrus_text* restrict this);
int chrus_text_get_height(chrus_text* restrict this);
bool chrus_text_get_visible(chrus_text* restrict this);

void chrus_text_set_color(chrus_text* restrict this, ALLEGRO_COLOR new);
void chrus_text_set_x(chrus_text* restrict this, float new);
void chrus_text_set_y(chrus_text* restrict this, float new);
void chrus_text_set_max_width(chrus_text* restrict this, float new);
void chrus_text_set_line_height(chrus_text* restrict this, float new);
void chrus_text_set_flags(chrus_text* restrict this, int new);
void chrus_text_set_text(chrus_text* restrict this, const char* new_text);
void chrus_text_set_font(chrus_text* restrict this, const char* font_path, int size);
void chrus_text_set_visible(chrus_text* restrict this, bool new);