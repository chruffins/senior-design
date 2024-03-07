#include "../include/primitive.h"

chrus_prim* chrus_prim_create() {
    chrus_prim* new_prim = malloc(sizeof(chrus_prim));

    if (new_prim == NULL) return NULL;
    memset(new_prim, 0, sizeof(new_prim));

    /* initializes to a line... just because. */
    new_prim->type = CHRUS_PRIMITIVE_HIGHLEVEL;
    new_prim->high_level.hl_type = CHRUS_PRIM_HL_LINE;

    return new_prim;
}

void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data) {
    this->type = CHRUS_PRIMITIVE_LOWLEVEL;

    this->low_level.vertex_buffer = al_create_vertex_buffer(NULL, init_data, num_vertices, 0);
    if (this->low_level.vertex_buffer == NULL) {
        /* need to fallback. TODO: make sure this failure only happens once */
        this->low_level.vertices = malloc(sizeof(ALLEGRO_VERTEX)*num_vertices);
        if (this->low_level.vertices == NULL) return; /* TODO: we should just crash the program at this point */
    }

    this->low_level.count = num_vertices;
}