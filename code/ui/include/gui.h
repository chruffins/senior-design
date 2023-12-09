#ifndef CHRUSGUI
#define CHRUSGUI

#include<allegro5/allegro5.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_primitives.h>
#include<stdbool.h>

/*
struct, union, enum declarations required
*/

enum GUI_BITMAP_MODE {
    GUI_BITMAP_CENTER,
    GUI_BITMAP_FILL,
    GUI_BITMAP_STRETCH,
    GUI_BITMAP_SCALE,
    GUI_BITMAP_TILE,
} typedef GUI_BITMAP_MODE;

enum GUI_TYPE {
    GUI_TYPE_LABEL,
    GUI_TYPE_BITMAP
} typedef GUI_TYPE;

struct gui_dims {
    float ax;
    float ay;
    float ox;
    float oy;
} typedef gui_dims;

struct gui_point {
    float x;
    float y;
} typedef gui_point;

struct gui_label {
    char *text;
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR font_color;
} typedef gui_label;

struct gui_bitmap {
    ALLEGRO_BITMAP *bitmap;
    /* need an enum for mode or something */
    GUI_BITMAP_MODE fill_mode;
    float rotation;
} typedef gui_bitmap;

typedef struct gui_element gui_element;

struct gui_element {
    int type;

    /* some obvious properties all gui elements need */
    gui_dims pos;
    gui_dims size;
    float x, y; // calculated values
    bool visible;
    bool enabled;

    void (*process_input)(gui_element*);
    void (*update)(gui_element*, double);
    void (*draw)(gui_element*);

    /* all gui elements inherit background properties too */
    ALLEGRO_COLOR background_color;
    float background_transparency; /* [0, 1] */

    ALLEGRO_COLOR border_color;
    float border_transparency; /* [0, 1] */

    union {
        gui_label label;
        gui_bitmap bitmap;
    };
} typedef gui_element;


/*
function declarations.
*/

gui_dims gui_pos_create(float ax, float ay, float ox, float oy);
gui_point gui_pos_to_point(gui_dims pos, int scw, int sch);
int gui_dim_to_point(int total, float absolute, int offset);

gui_element gui_label_create();
gui_element gui_bitmap_create();

ALLEGRO_FONT *gui_get_debug_font();

void gui_element_move(gui_element*, gui_dims);

void gui_bitmap_change_bitmap(gui_element*, ALLEGRO_BITMAP*);
void gui_bitmap_change_bitmap_fillmode(gui_element*, GUI_BITMAP_MODE);

void gui_bitmap_update_bitmap_draw(gui_element*);

void gui_label_draw(gui_element*);
void gui_bitmap_draw(gui_element*);

#endif