#pragma once

/*         _________________________________________   ________________
 *        /          /    /   /    /          /    /  /    /          /
 *       /    ______/    /   /    /    ______/    /  /    /   _______/
 *      /    /     /    /___/    /    /     /    /  /    /\       \
 *     /    /     /    ____     /    /     /    /  /    /  \___    \
 *    /    /_____/    /   /    /    /     /    /__/    /______/    /
 *   /          /    /   /    /    /     /            /           /
 *   \_________/____/   /____/____/     /____________/___________/
 * 
 * Vector implementation.
*/
#pragma once

#include<memory.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct chrus_vector_t chrus_vector;

struct chrus_vector_t {
    void **data;
    size_t size;
    size_t capacity;
};

chrus_vector chrus_vector_create();

bool chrus_vector_append(chrus_vector *this, void *data);
void* chrus_vector_pop(chrus_vector *this);
bool chrus_vector_remove(chrus_vector *this, void *match);
bool chrus_vector_insert(chrus_vector *this, void *data, size_t position);
bool chrus_vector_reserve(chrus_vector* restrict this, size_t new_capacity);

void chrus_vector_destroy(chrus_vector *this);