/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Defines the primitive struct, which can be used to draw vector
 * graphics. Also provides extra utility like calculating rotations
 * for you.
*/
#pragma once

#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>

typedef struct chrus_primitive_t chrus_prim;

struct chrus_primitive_t {
    union {
        struct prim {
            ALLEGRO_VERTEX* vertices;
            ALLEGRO_BITMAP* texture;
            int start;
            int end;
            int type;
            int count; /* number of vertices so you can't accidentally crash the game */
        };
        
    };
};