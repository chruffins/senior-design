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
#include<string.h>

typedef struct chrus_primitive_t chrus_prim;
typedef enum CHRUS_PRIMITIVE_TYPE CHRUS_PRIMITIVE_TYPE;
typedef enum CHRUS_PRIM_HL_TYPE CHRUS_PRIM_HL_TYPE;

enum CHRUS_PRIMITIVE_TYPE {
    CHRUS_PRIMITIVE_HIGHLEVEL,
    CHRUS_PRIMITIVE_LOWLEVEL
};

enum CHRUS_PRIM_HL_TYPE {
    CHRUS_PRIM_HL_LINE,
    CHRUS_PRIM_HL_TRIANGLE,
    CHRUS_PRIM_HL_FILLED_TRIANGLE,
    CHRUS_PRIM_HL_RECTANGLE,
    CHRUS_PRIM_HL_FILLED_RECTANGLE,
    CHRUS_PRIM_HL_ROUNDED_RECTANGLE,
    CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE,
    CHRUS_PRIM_HL_PIESLICE,
    CHRUS_PRIM_HL_FILLED_PIESLICE,
    CHRUS_PRIM_HL_ELLIPSE,
    CHRUS_PRIM_HL_FILLED_ELLIPSE,
    CHRUS_PRIM_HL_CIRCLE,
    CHRUS_PRIM_HL_FILLED_CIRCLE,
    CHRUS_PRIM_HL_ARC,
    CHRUS_PRIM_HL_ELLIPTICAL_ARC,
    CHRUS_PRIM_HL_SPLINE,
};

struct chrus_primitive_t {
    CHRUS_PRIMITIVE_TYPE type; /* probably should be erroneous to change this */
    union {
        struct high_level {
            CHRUS_PRIM_HL_TYPE hl_type; 
            float x1;
            float y1;
            float x2;
            float y2;
            float x3;
            float y3;
            float rx; /* x4 as necessary */
            float ry; /* y4 as necessary */
            float thickness;
            ALLEGRO_COLOR color;
        } high_level;
        struct low_level {
            ALLEGRO_VERTEX_BUFFER* vertex_buffer;
            ALLEGRO_VERTEX* vertices; /* alternative if vertex_buffer fails to create? */
            ALLEGRO_BITMAP* texture;
            int start;
            int end;
            int type;
            int count; /* number of vertices so you can't accidentally crash the game */
            ALLEGRO_LINE_JOIN line_style; /* */
        } low_level;
    };
};

chrus_prim* chrus_prim_create();
void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data);