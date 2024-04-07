/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This header stores functions for creating or loading bitmaps in-engine.
 * This is because bitmaps are loaded in scene threads, but need to be converted
 * to video bitmaps by the drawing thread. So, we create the bitmaps in the scene thread
 * but send events to the drawing thread to convert the bitmaps (which transfers ownership)
 * at the same time.
*/
#pragma once
#include<allegro5/allegro.h>

#include "globals.h"
#include "enums.h"

/* these functions tell the drawing thread to do stuff */
ALLEGRO_BITMAP* chrus_create_bitmap(int w, int h);
ALLEGRO_BITMAP* chrus_load_bitmap(const char* filename);
ALLEGRO_SHADER* chrus_shader_create();

/* asynchronously tells the drawing thread to do stuff and is guaranteed to be done in order */
void chrus_shader_attach_source_file(ALLEGRO_SHADER* restrict shader, ALLEGRO_SHADER_TYPE type, const char* filename);
void chrus_shader_attach_source(ALLEGRO_SHADER* restrict shader, ALLEGRO_SHADER_TYPE type, const char* source);
void chrus_shader_build(ALLEGRO_SHADER* shader);

/* some misc functions */
void chrus_set_window_title(const char* new_title);
void chrus_set_window_icon(ALLEGRO_BITMAP* icon);