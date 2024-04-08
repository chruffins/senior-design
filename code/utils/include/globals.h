/*                     _________________________________________   ________________
 *                    /          /    /   /    /          /    /  /    /          /
 *                   /    ______/    /   /    /    ______/    /  /    /   _______/
 *                  /    /     /    /___/    /    /     /    /  /    /\       \
 *                 /    /     /    ____     /    /     /    /  /    /  \___    \
 *                /    /_____/    /   /    /    /     /    /__/    /______/    /
 *               /          /    /   /    /    /     /            /           /
 *               \_________/____/   /____/____/     /____________/___________/
 * 
 *         ______________      ________________________________      ________________
 *        /          /   \    /    /           /           /   \    /    /          /
 *       /    ______/     \  /    /    _______/___    ____/     \  /    /    ______/
 *      /          /       \/    /    /      /   /   /   /       \/    /          /
 *     /          /             /    /  /   /   /   /   /             /          /
 *    /    ______/    /\       /    /__/   /___/   /___/    /\       /    ______/
 *   /          /    /  \     /           /           /    /  \     /          /
 *   \_________/____/    \___/___________/___________/____/    \___/\_________/
 * Please forgive me.
*/
#pragma once
#include<allegro5/allegro.h>
#include<stdio.h>

#include "enums.h"

extern ALLEGRO_DISPLAY* chrus_display;
extern ALLEGRO_EVENT_QUEUE* chrus_drawing_queue;
extern ALLEGRO_EVENT_SOURCE chrus_drawing_event_source;
extern float audio_buffer[1024];

float* chrus_demo_get_audio_buffer();
void chrus_demo_postprocess_callback(void* buf, unsigned int samples, void* data);
void chrus_join_drawing_thread();