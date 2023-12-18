#include "../include/vector.h"

chrus_vector chrus_vector_create() {
    return (chrus_vector){ malloc(sizeof(void*)), 0, 1 };
}

void chrus_vector_append(chrus_vector *this, void *data) {
    if (this->size + 1 == this->capacity) {
        void *newdata = realloc(this->data, this->capacity * sizeof(void*) * 2);
        if (newdata == NULL) {
            // TODO: some kind of way to tell the world that we fucked up
        }
        this->capacity *= 2;
        this->data = newdata;
    }
    this->data[this->size] = data;
    this->size++;
}

void* chrus_vector_pop(chrus_vector *this) {
    if (this->size == 0) return NULL; // TODO: tell that it didn't work

    void *ret = this->data[this->size];

    this->data[this->size] = NULL;
    this->size--;

    return ret;
}

void chrus_vector_remove(chrus_vector *this, void *match);
void chrus_vector_insert(chrus_vector *this, void *data, size_t position);

void chrus_vector_destroy(chrus_vector *this) {
    if (this->capacity == 0) return;
    // this vector only stores references, so we should NOT be freeing any of our pointers.
    free(this->data);
    this->size = 0;
    this->capacity = 0;
}