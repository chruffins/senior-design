#include "../include/primitive.h"

static void chrus_prim_draw_highlevel(chrus_prim* restrict this, float dx, float dy);
static void chrus_prim_draw_lowlevel(chrus_prim* restrict this, float dx, float dy);
static void chrus_prim_draw_lowlevel_backup(chrus_prim* restrict this, float dx, float dy);

chrus_prim* chrus_prim_create() {
    chrus_prim* new_prim = malloc(sizeof(chrus_prim));

    if (new_prim == NULL) return NULL;
    memset(new_prim, 0, sizeof(new_prim));

    /* initializes to a line... just because. */
    new_prim->type = CHRUS_PRIMITIVE_HIGHLEVEL;
    new_prim->hl.hl_type = CHRUS_PRIM_HL_LINE;

    return new_prim;
}

void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data) {
    this->type = CHRUS_PRIMITIVE_LOWLEVEL;

    this->ll.vertex_buffer = al_create_vertex_buffer(NULL, init_data, num_vertices, 0);
    if (this->ll.vertex_buffer == NULL) {
        this->type = CHRUS_PRIMITIVE_LOWLEVEL_BACKUP;
        /* need to fallback. TODO: make sure this failure only happens once */
        this->ll.vertex_backup = malloc(sizeof(ALLEGRO_VERTEX)*num_vertices);
        if (this->ll.vertex_backup == NULL) return; /* TODO: we should just crash the program at this point */
    }

    this->ll.count = num_vertices;
}

void chrus_prim_create_hl(chrus_prim* restrict this) {
    if (this->type == CHRUS_PRIMITIVE_LOWLEVEL || this->type == CHRUS_PRIMITIVE_LOWLEVEL_BACKUP) {
        if (this->ll.vertex_buffer) al_destroy_vertex_buffer(this->ll.vertex_buffer);
        if (this->ll.vertex_backup) free(this->ll.vertex_backup);
    }

    this->type = CHRUS_PRIMITIVE_HIGHLEVEL;
    memset(&this->hl, 0, sizeof(this->hl));
    this->hl.hl_type = CHRUS_PRIM_HL_LINE;
}

void chrus_prim_draw_highlevel(chrus_prim* restrict this, float dx, float dy) {
    switch (this->hl.hl_type)
    {
    case CHRUS_PRIM_HL_LINE:
        al_draw_line(this->hl.x1 + dx, this->hl.y1 + dy, 
            this->hl.x2 + dx, this->hl.y2 + dy, 
            this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_TRIANGLE:
        al_draw_triangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx,
            this->hl.y2 + dy, this->hl.x3 + dx, this->hl.y3 + dy, this->hl.color,
            this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_TRIANGLE:
        al_draw_filled_triangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx,
            this->hl.y2 + dy, this->hl.x3 + dx, this->hl.y3 + dy, this->hl.color);
        break;
    case CHRUS_PRIM_HL_RECTANGLE:
        al_draw_rectangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx, 
            this->hl.y2 + dy, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_RECTANGLE:
        al_draw_filled_rectangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx, 
            this->hl.y2 + dy, this->hl.color);
        break;
    case CHRUS_PRIM_HL_ROUNDED_RECTANGLE:
        al_draw_rounded_rectangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx, 
            this->hl.y2 + dy, this->hl.rx, this->hl.ry, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE:
        al_draw_filled_rounded_rectangle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.x2 + dx, 
            this->hl.y2 + dy, this->hl.rx, this->hl.ry, this->hl.color);
        break;
    case CHRUS_PRIM_HL_PIESLICE:
        al_draw_pieslice(this->hl.x1 + dx, this->hl.y1, this->hl.rx, this->hl.x2, 
            this->hl.y2, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_PIESLICE:
        al_draw_filled_pieslice(this->hl.x1 + dx, this->hl.y1, this->hl.rx, this->hl.x2, 
            this->hl.y2, this->hl.color);
        break;
    case CHRUS_PRIM_HL_ELLIPSE:
        al_draw_ellipse(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.ry, 
            this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_ELLIPSE:
        al_draw_filled_ellipse(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.ry, 
            this->hl.color);
        break;
    case CHRUS_PRIM_HL_CIRCLE:
        al_draw_circle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_FILLED_CIRCLE:
        al_draw_filled_circle(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.color);
        break;
    case CHRUS_PRIM_HL_ARC:
        al_draw_arc(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.x2, 
            this->hl.y2, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_ELLIPTICAL_ARC:
        al_draw_elliptical_arc(this->hl.x1 + dx, this->hl.y1 + dy, this->hl.rx, this->hl.ry, this->hl.x2, 
            this->hl.y2, this->hl.color, this->hl.thickness);
        break;
    case CHRUS_PRIM_HL_SPLINE:
        al_draw_spline(&this->hl.x1, this->hl.color, this->hl.thickness);
        break;
    default:
        break;
    }
}

void chrus_prim_draw_lowlevel(chrus_prim* restrict this, float dx, float dy) {
    al_draw_vertex_buffer(this->ll.vertex_buffer, this->ll.texture, this->ll.start, this->ll.end, this->ll.type);
}

void chrus_prim_draw_lowlevel_backup(chrus_prim* restrict this, float dx, float dy) {
    al_draw_prim(this->ll.vertex_backup, NULL, this->ll.texture, this->ll.start, this->ll.end, this->ll.type);
}

void chrus_prim_draw(chrus_prim* restrict this, float dx, float dy) {
    switch (this->type)
    {
    case CHRUS_PRIMITIVE_HIGHLEVEL:
        chrus_prim_draw_highlevel(this, dx, dy);
        break;
    case CHRUS_PRIMITIVE_LOWLEVEL:
        chrus_prim_draw_lowlevel(this, dx, dy);
        break;
    case CHRUS_PRIMITIVE_LOWLEVEL_BACKUP:
        chrus_prim_draw_lowlevel_backup(this, dx, dy);
        break;
    default:
        break;
    }
}



/* begins the boilerplate */
bool chrus_prim_set_line(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_LINE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.thickness = thickness;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float thickness, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_TRIANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.x3 = x3;
    this->hl.y3 = y3;
    this->hl.thickness = thickness;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_filled_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_TRIANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.x3 = x3;
    this->hl.y3 = y3;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_RECTANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.thickness = thickness;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_filled_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_RECTANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, float thickness, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_ROUNDED_RECTANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.rx = rx;
    this->hl.ry = ry;
    this->hl.thickness = thickness;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_filled_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.rx = rx;
    this->hl.ry = ry;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_PIESLICE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = start_theta;
    this->hl.y2 = end_theta;
    this->hl.rx = rx;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}

bool chrus_prim_set_filled_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_PIESLICE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = start_theta;
    this->hl.y2 = end_theta;
    this->hl.rx = rx;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_ELLIPSE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.rx = rx;
    this->hl.ry = ry;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}

bool chrus_prim_set_filled_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_ELLIPSE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.rx = rx;
    this->hl.ry = ry;
    this->hl.color = color;

    return true;
}

bool chrus_prim_set_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_CIRCLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.rx = rx;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}

bool chrus_prim_set_filled_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_FILLED_CIRCLE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.rx = rx;
    this->hl.color = color;

    return true;
}
bool chrus_prim_set_arc(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_ARC;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = start_theta;
    this->hl.y2 = end_theta;
    this->hl.rx = rx;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}

bool chrus_prim_set_elliptical_arc(chrus_prim* restrict this, float x1, float y1, float rx, float ry, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_ELLIPTICAL_ARC;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = start_theta;
    this->hl.y2 = end_theta;
    this->hl.rx = rx;
    this->hl.ry = ry;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}

bool chrus_prim_set_spline(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, ALLEGRO_COLOR color, float thickness) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;
    
    this->hl.hl_type = CHRUS_PRIM_HL_SPLINE;

    this->hl.x1 = x1;
    this->hl.y1 = y1;
    this->hl.x2 = x2;
    this->hl.y2 = y2;
    this->hl.x3 = x3;
    this->hl.y3 = y3;
    this->hl.rx = x4;
    this->hl.ry = y4;
    this->hl.color = color;
    this->hl.thickness = thickness;

    return true;
}