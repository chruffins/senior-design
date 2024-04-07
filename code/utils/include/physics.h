/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * An attempt at physics.
*/
#pragma once

#include<allegro5/allegro.h>

typedef struct chrus_vec2 chrus_vec2;
typedef struct chrus_vec3 chrus_vec3;
typedef struct chrus_vec4 chrus_vec4;

struct chrus_vec2 {
    float x;
    float y;
};

struct chrus_vec3 {
    float x;
    float y;
    float z;
};

struct chrus_vec4 {
    float x;
    float y;
    float z;
    float a;
};

inline chrus_vec2 chrus_vec2_create(float x, float y) {
    return (chrus_vec2){ .x = x, .y = y };
}

inline chrus_vec3 chrus_vec3_create(float x, float y, float z) {
    return (chrus_vec3){ .x = x, .y = y, .z = z };
}

inline chrus_vec4 chrus_vec4_create(float x, float y, float z, float a) {
    return (chrus_vec4){ .x = x, .y = y, .z = z, .a = a };
}

inline bool chrus_rectangles_collide(chrus_vec2 r1p, chrus_vec2 r1s, chrus_vec2 r2p, chrus_vec2 r2s) {
    if (r1p.x + r1s.x >= r2p.x && r1p.x <= r2p.x + r2s.x
        && r1p.y + r1s.y >= r2p.y && r1p.y <= r2p.y + r2s.y) {
        return true;
    }

    return false;
}

void* physics_handler(ALLEGRO_THREAD*, void*);