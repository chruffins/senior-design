/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * This header stores the allocations of assets loaded into the game using
 * static red black trees. The load function of the game objects
 * will rely on the loading in of assets... There should be some kind
 * of garbage collection mechanic as well. Probably when scenes are
 * deloaded.
*/
#pragma once
#include<string.h>

#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>

#include "rbtree.h"

typedef enum CHRUS_LOADER CHRUS_LOADER;

enum CHRUS_LOADER {
    CHRUS_LOADER_BITMAP=0,
    CHRUS_LOADER_SAMPLE,
    CHRUS_LOADER_SCRIPT,
    CHRUS_LOADER_END
};

void chrus_loader_init();
void *chrus_loader_insert(CHRUS_LOADER, const void *key);
void *chrus_loader_get(CHRUS_LOADER, const void *key);
void chrus_loader_deinit();