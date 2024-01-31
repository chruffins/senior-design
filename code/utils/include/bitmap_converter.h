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

#include "drawing_thread.h"

/* these functions tell the drawing thread to do stuff */
ALLEGRO_BITMAP* chrus_create_bitmap(int w, int h);
ALLEGRO_BITMAP* chrus_load_bitmap(const char* filename);