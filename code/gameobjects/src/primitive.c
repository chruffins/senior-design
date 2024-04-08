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
    new_prim->visible = true;

    return new_prim;
}

void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data, bool backup) {
    this->ll.vertex_buffer = NULL;
    this->type = CHRUS_PRIMITIVE_LOWLEVEL;
    this->ll.count = num_vertices;
    this->ll.texture = NULL;
    this->ll.start = 0;
    this->ll.end = num_vertices;
    
    if (backup == false) this->ll.vertex_buffer = al_create_vertex_buffer(NULL, init_data, num_vertices, 0);
    if (this->ll.vertex_buffer == NULL) {
        this->type = CHRUS_PRIMITIVE_LOWLEVEL_BACKUP;
        /* need to fallback. TODO: make sure this failure only happens once */
        this->ll.vertex_backup = calloc(num_vertices, sizeof(ALLEGRO_VERTEX));
        if (this->ll.vertex_backup == NULL) return; /* TODO: we should just crash the program at this point */
        //printf("created a backup\n");
    }
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

void chrus_prim_destroy(chrus_prim* restrict this) {
    if (this->type == CHRUS_PRIMITIVE_HIGHLEVEL) {
        free(this);
        return;
    }

    if (this->type == CHRUS_PRIMITIVE_LOWLEVEL && this->ll.vertex_buffer) {
        al_destroy_vertex_buffer(this->ll.vertex_buffer);
    } else if (this->type == CHRUS_PRIMITIVE_LOWLEVEL_BACKUP && this->ll.vertex_backup) {
        free(this->ll.vertex_backup);
    }
    free(this);
    return;
}

bool chrus_prim_get_visible(chrus_prim* restrict this) {
    return this->visible;
}

int  chrus_prim_get_layer(chrus_prim* restrict this) {
    return this->_layer;
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
        al_draw_spline((float*)&this->hl.x1, this->hl.color, this->hl.thickness);
        break;
    default:
        break;
    }
}

void chrus_prim_draw_lowlevel(chrus_prim* restrict this, float dx, float dy) {
    if (this->ll.vertex_buffer == NULL) return;

    al_draw_vertex_buffer(this->ll.vertex_buffer, this->ll.texture, this->ll.start, this->ll.end, this->ll.type);
}

void chrus_prim_draw_lowlevel_backup(chrus_prim* restrict this, float dx, float dy) {
    if (this->ll.vertex_backup == NULL) return;
    
    al_draw_prim(this->ll.vertex_backup, NULL, this->ll.texture, this->ll.start, this->ll.end, this->ll.type);
}

void chrus_prim_draw(chrus_prim* restrict this, float dx, float dy) {
    if (!this->visible) return;

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

bool chrus_prim_translate(chrus_prim* restrict this, float dx, float dy) {
    /* guhhh yeah i don't know what to do for low level yet lmao 
        TODO: figure out low-level translate
    */
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    switch (this->hl.hl_type)
    {
    /* one point to translate */
    case CHRUS_PRIM_HL_CIRCLE:
    case CHRUS_PRIM_HL_FILLED_CIRCLE:
    case CHRUS_PRIM_HL_ELLIPSE:
    case CHRUS_PRIM_HL_FILLED_ELLIPSE:
    case CHRUS_PRIM_HL_ARC:
    case CHRUS_PRIM_HL_ELLIPTICAL_ARC:
    case CHRUS_PRIM_HL_PIESLICE:
    case CHRUS_PRIM_HL_FILLED_PIESLICE:
        this->hl.x1 += dx;
        this->hl.y1 += dy;
        break;
    /* two points to translate */
    case CHRUS_PRIM_HL_LINE:
    case CHRUS_PRIM_HL_RECTANGLE:
    case CHRUS_PRIM_HL_FILLED_RECTANGLE:
    case CHRUS_PRIM_HL_ROUNDED_RECTANGLE:
    case CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE:
        this->hl.x1 += dx;
        this->hl.y1 += dy;
        this->hl.x2 += dx;
        this->hl.y2 += dy;
        break;
    /* three points to translate */
    case CHRUS_PRIM_HL_TRIANGLE:
    case CHRUS_PRIM_HL_FILLED_TRIANGLE:
        this->hl.x1 += dx;
        this->hl.y1 += dy;
        this->hl.x2 += dx;
        this->hl.y2 += dy;
        this->hl.x3 += dx;
        this->hl.y3 += dy;
        break;
    /* four points to translate */
    case CHRUS_PRIM_HL_SPLINE:
        this->hl.x1 += dx;
        this->hl.y1 += dy;
        this->hl.x2 += dx;
        this->hl.y2 += dy;
        this->hl.x3 += dx;
        this->hl.y3 += dy;
        this->hl.rx += dx;
        this->hl.ry += dy;
        break;
    default:
        printf("translating an invalid primitive type?\n");
        break;
    }

    return true;
}

bool chrus_prim_get_filled(chrus_prim* restrict this) {
    /* this only applies to highlevel because getting filled doesn't make sense for lowlevel */
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    switch (this->hl.hl_type)
    {
    case CHRUS_PRIM_HL_FILLED_CIRCLE:
    case CHRUS_PRIM_HL_FILLED_ELLIPSE:
    case CHRUS_PRIM_HL_FILLED_PIESLICE:
    case CHRUS_PRIM_HL_FILLED_RECTANGLE:
    case CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE:
    case CHRUS_PRIM_HL_FILLED_TRIANGLE:
        return true;
    default:
        return false;
    }
}

bool chrus_prim_set_visible(chrus_prim* restrict this, bool new) {
    this->visible = new;

    return true;
}

bool chrus_prim_set_filled(chrus_prim* restrict this, bool new_value) {
    /* this only applies to highlevel because setting filled doesn't make sense for lowlevel */
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    if (new_value) {
        switch (this->hl.hl_type)
        {
        case CHRUS_PRIM_HL_CIRCLE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_CIRCLE;
            break;
        case CHRUS_PRIM_HL_ELLIPSE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_ELLIPSE;
            break;
        case CHRUS_PRIM_HL_PIESLICE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_PIESLICE;
            break;
        case CHRUS_PRIM_HL_RECTANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_RECTANGLE;
            break;
        case CHRUS_PRIM_HL_ROUNDED_RECTANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE;
            break;
        case CHRUS_PRIM_HL_TRIANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_FILLED_TRIANGLE;
            break;
        default:
            /* setting to be filled on filled OR unfillable types does nothing */
            return false;
        }
    } else {
        switch (this->hl.hl_type)
        {
        case CHRUS_PRIM_HL_FILLED_CIRCLE:
            this->hl.hl_type = CHRUS_PRIM_HL_CIRCLE;
            break;
        case CHRUS_PRIM_HL_FILLED_ELLIPSE:
            this->hl.hl_type = CHRUS_PRIM_HL_ELLIPSE;
            break;
        case CHRUS_PRIM_HL_FILLED_PIESLICE:
            this->hl.hl_type = CHRUS_PRIM_HL_PIESLICE;
            break;
        case CHRUS_PRIM_HL_FILLED_RECTANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_RECTANGLE;
            break;
        case CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_ROUNDED_RECTANGLE;
            break;
        case CHRUS_PRIM_HL_FILLED_TRIANGLE:
            this->hl.hl_type = CHRUS_PRIM_HL_TRIANGLE;
            break;
        default:
            /* setting to not be filled on non-filled types does nothing */
            return false;
        }
    }

    /* indicates that the function was successful */
    return true;
}

bool chrus_prim_set_hl_type(chrus_prim* restrict this, CHRUS_PRIM_HL_TYPE type) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    this->hl.hl_type = type;
    return true;
}

bool chrus_prim_set_hl_value(chrus_prim* restrict this, int ptr, float value) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    switch (ptr)
    {
    case 0:
        this->hl.x1 = value;
        break;
    case 1:
        this->hl.y1 = value;
        break;
    case 2:
        this->hl.x2 = value;
        break;
    case 3:
        this->hl.y2 = value;
        break;
    case 4:
        this->hl.x3 = value;
        break;
    case 5:
        this->hl.y3 = value;
        break;
    case 6:
        this->hl.rx = value;
        break;
    case 7:
        this->hl.ry = value;
        break;
    case 8:
        this->hl.thickness = value;
        break;
    default:
        break;
    }

    return true;
}

bool chrus_prim_set_color(chrus_prim* restrict this, ALLEGRO_COLOR color) {
    if (this->type != CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    this->hl.color = color;

    return true;
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

bool chrus_prim_set_type(chrus_prim* restrict this, int new) {
    if (this->type == CHRUS_PRIMITIVE_HIGHLEVEL) return false;

    this->ll.type = new;
    return true;
}

ALLEGRO_VERTEX* chrus_prim_get_vertices(chrus_prim* restrict this) {
    if (this->type != CHRUS_PRIMITIVE_LOWLEVEL_BACKUP) return NULL;

    return this->ll.vertex_backup;
}