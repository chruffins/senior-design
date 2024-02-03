#include "../include/text.h"

static ALLEGRO_FONT* default_font = NULL;

static ALLEGRO_FONT* get_default_font() {
    if (!default_font) {
        default_font = al_create_builtin_font();
    }
    return default_font;
}

chrus_text* chrus_text_create() {
    chrus_text* new_text = malloc(sizeof(chrus_text));
    new_text->font = get_default_font();
    new_text->color = al_map_rgb(255, 255, 255);
    new_text->x = 100;
    new_text->y = 100;
    new_text->max_width = 400;
    new_text->line_height = 0;
    new_text->flags = ALLEGRO_ALIGN_LEFT;
    new_text->text = "Hello world!";

    return new_text;
}

void chrus_text_draw(chrus_text* restrict this, float dx, float dy) {
    al_draw_multiline_text(this->font, this->color, this->x + dx, this->y + dy, this->max_width, this->line_height, this->flags, this->text);
}

void chrus_text_destroy(chrus_text* restrict this) {
    free(this);
}

const char* chrus_text_get_text(chrus_text* restrict this) {
    return this->text;
}

void chrus_text_set_text(chrus_text* restrict this, const char* new_text) {
    this->text = new_text;
}