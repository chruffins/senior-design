/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Please forgive me.
*/
#pragma once
#include<allegro5/allegro.h>
#include<stdio.h>

#include "enums.h"

extern ALLEGRO_DISPLAY* chrus_display;
extern ALLEGRO_EVENT_QUEUE* chrus_drawing_queue;
extern ALLEGRO_EVENT_SOURCE chrus_drawing_event_source;

void chrus_join_drawing_thread();