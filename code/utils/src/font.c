#include "../include/font.h"

static struct chrus_fontsize_elem_t chrus_create_fontsize_elem(chrus_font* restrict this, int size);

ALLEGRO_FONT* chrus_font_append_size(chrus_font* restrict this, int size) {
    ALLEGRO_FONT* font = NULL;
    if ((font = chrus_font_get_size(this, size)) != NULL) return font;

    if (size <= 0) return NULL;

    if (this->sizes == NULL) {
        this->sizes = malloc(sizeof(struct chrus_fontsize_elem_t));
        this->capacity = 1;
        this->size = 1;
        this->sizes[0] = chrus_create_fontsize_elem(this, size);
        return this->sizes[0].font;
    }

    if (this->size + 1 >= this->capacity) {
        struct chrus_fontsize_elem_t* newdata = realloc(this->sizes, this->capacity * sizeof(struct chrus_fontsize_elem_t) * 2);
        if (newdata == NULL) {
            // TODO: some kind of way to tell the world that we fucked up
            return NULL;
        }
        this->capacity *= 2;
        this->sizes = newdata;
    }
    this->sizes[this->size] = chrus_create_fontsize_elem(this, size);
    this->size++;

    font = this->sizes[this->size - 1].font;
    return font;
}

static struct chrus_fontsize_elem_t chrus_create_fontsize_elem(chrus_font* restrict this, int size) {
    return (struct chrus_fontsize_elem_t){ .size = size, .font = al_load_ttf_font(this->source, size, 0)};
}

chrus_font* chrus_font_create(const char* source) {
    if (!al_filename_exists(source)) return NULL;

    chrus_font* font = malloc(sizeof(chrus_font));
    font->source = source;

    font->sizes = malloc(sizeof(struct chrus_fontsize_elem_t));
    font->capacity = 1;
    font->size = 0;

    return font;
}

void chrus_font_destroy(chrus_font* restrict this) {
    for (int i = 0; i < this->size; i++) {
        al_destroy_font(this->sizes[i].font); 
    }
    free(this->sizes);

    free(this);
}

ALLEGRO_FONT* chrus_font_get_size(chrus_font* restrict this, int size) {
    for (int i = 0; i < this->size; i++) {
        if (this->sizes[i].size == size) return this->sizes[i].font;
    }

    return NULL;
}