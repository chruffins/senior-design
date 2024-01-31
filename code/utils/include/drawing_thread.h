/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This header stores the functionality for the drawing thread. All
 * bitmaps are owned by the display, which is owned by a single thread.
 * I want to avoid giving threads the context as necessary... because
 * I'm not entirely sure how that works.
*/
#pragma once
#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>

#include "../../gameobjects/include/scenemanager.h"
#include "utils.h"

extern ALLEGRO_DISPLAY* chrus_display;
extern ALLEGRO_EVENT_QUEUE* chrus_drawing_queue;
extern ALLEGRO_EVENT_SOURCE chrus_drawing_event_source;

/* all drawing calls are handled in this thread, and bitmaps need to be converted in here */
void* drawing_handler(ALLEGRO_THREAD *this, void *args);