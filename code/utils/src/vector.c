#include "../include/vector.h"

chrus_vector chrus_vector_create() {
    return (chrus_vector){ NULL, 0, 0 };
}

bool chrus_vector_append(chrus_vector *this, void *data) {
    if (this->data == NULL) {
        this->data = malloc(sizeof(void*));
        this->capacity = 1;
        this->size = 1;
        this->data[0] = data;
        return true;
    }

    if (this->size + 1 >= this->capacity) {
        void** newdata = realloc(this->data, this->capacity * sizeof(void*) * 2);
        if (newdata == NULL) {
            // TODO: some kind of way to tell the world that we fucked up
            return false;
        }
        this->capacity *= 2;
        this->data = newdata;
    }
    this->data[this->size] = data;
    this->size++;

    return true;
}

void* chrus_vector_pop(chrus_vector *this) {
    if (this->size == 0) return NULL; // TODO: tell that it didn't work

    void *ret = this->data[this->size];

    this->data[this->size] = NULL;
    this->size--;

    return ret;
}

bool chrus_vector_remove(chrus_vector *this, void *match);
bool chrus_vector_insert(chrus_vector *this, void *data, size_t position);

bool chrus_vector_reserve(chrus_vector* restrict this, size_t new_capacity) {
    if (new_capacity <= this->capacity) return false;

    if (this->data == NULL) {
        this->data = malloc(sizeof(void*) * new_capacity);
        this->capacity = new_capacity;
        this->size = 0;
        return true;
    } else {
        void** new_data = realloc(this->data, new_capacity);
        if (new_data == NULL) return false;
        this->data = new_data;
        this->capacity = new_capacity;
        /* unchanged size */
        return true;
    }
}

void chrus_vector_destroy(chrus_vector *this) {
    if (this->capacity == 0) return;
    // this vector only stores references, so we should NOT be freeing any of our pointers.
    free(this->data);
    this->size = 0;
    this->capacity = 0;
}