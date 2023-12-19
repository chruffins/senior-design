#pragma once

#include<memory.h>
#include<stdlib.h>

typedef struct chrus_vector_t chrus_vector;

struct chrus_vector_t {
    void **data;
    size_t size;
    size_t capacity;
};

chrus_vector chrus_vector_create();

void chrus_vector_append(chrus_vector *this, void *data);
void* chrus_vector_pop(chrus_vector *this);
void chrus_vector_remove(chrus_vector *this, void *match);
void chrus_vector_insert(chrus_vector *this, void *data, size_t position);

void chrus_vector_destroy(chrus_vector *this);