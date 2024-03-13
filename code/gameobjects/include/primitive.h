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
#include<stdio.h>

typedef struct chrus_primitive_t chrus_prim;
typedef enum CHRUS_PRIMITIVE_TYPE CHRUS_PRIMITIVE_TYPE;
typedef enum CHRUS_PRIM_HL_TYPE CHRUS_PRIM_HL_TYPE;

enum CHRUS_PRIMITIVE_TYPE {
    CHRUS_PRIMITIVE_HIGHLEVEL,
    CHRUS_PRIMITIVE_LOWLEVEL,
    CHRUS_PRIMITIVE_LOWLEVEL_BACKUP
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
        struct hl {
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
        } hl;
        struct ll {
            ALLEGRO_VERTEX_BUFFER* vertex_buffer;
            ALLEGRO_VERTEX* vertex_backup; /* alternative if vertex_buffer fails to create? */
            ALLEGRO_BITMAP* texture;
            int start;
            int end;
            int type;
            int count; /* number of vertices so you can't accidentally crash the game */
            ALLEGRO_LINE_JOIN line_style; /* */
        } ll;
    };
};

chrus_prim* chrus_prim_create();
void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data, bool backup);
void chrus_prim_create_hl(chrus_prim* restrict this);
void chrus_prim_destroy(chrus_prim* restrict this);

void chrus_prim_draw(chrus_prim* restrict this, float dx, float dy);

bool chrus_prim_translate(chrus_prim* restrict this, float dx, float dy);

bool chrus_prim_get_filled(chrus_prim* restrict this);
bool chrus_prim_set_filled(chrus_prim* restrict this, bool new_value);

bool chrus_prim_set_line(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, ALLEGRO_COLOR color);
bool chrus_prim_set_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, ALLEGRO_COLOR color);
bool chrus_prim_set_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, ALLEGRO_COLOR color);
bool chrus_prim_set_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color);
bool chrus_prim_set_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color);
bool chrus_prim_set_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color);
bool chrus_prim_set_arc(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_elliptical_arc(chrus_prim* restrict this, float x1, float y1, float rx, float ry, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_spline(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, ALLEGRO_COLOR color, float thickness);
