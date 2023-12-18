#include "../include/sprite.h"

static chrus_sprite* chrus_sprite_create_default() {
    static ALLEGRO_BITMAP* default_square;
    if (default_square == NULL) {
        default_square = al_create_bitmap(64, 64);
        ALLEGRO_BITMAP *temp = al_get_target_bitmap();
        al_set_target_bitmap(default_square);
        al_draw_filled_rectangle(0, 0, 64, 64, al_map_rgb(255, 255, 255));
        al_set_target_bitmap(temp);
    }

    chrus_sprite *new_sprite = malloc(sizeof(chrus_sprite));
    new_sprite->x = 0;
    new_sprite->y = 0;
    new_sprite->width = 64;
    new_sprite->height = 64;
    new_sprite->rotation = 0;
    new_sprite->flipping = 0;
    new_sprite->image_data = default_square;

    return new_sprite;
}

chrus_sprite* chrus_sprite_create(const char *source) {
    if (source == NULL) return chrus_sprite_create_default();

    ALLEGRO_BITMAP *loader = al_load_bitmap(source);
    if (!loader) return chrus_sprite_create_default();

    chrus_sprite *new_sprite = malloc(sizeof(chrus_sprite));

    new_sprite->x = 0;
    new_sprite->y = 0;
    new_sprite->width = al_get_bitmap_width(loader);
    new_sprite->height = al_get_bitmap_height(loader);
    new_sprite->rotation = 0;
    new_sprite->flipping = 0;
    new_sprite->image_data = loader;

    return new_sprite;
}

void chrus_sprite_draw(chrus_sprite *this) {
    al_draw_bitmap(this->image_data, this->x, this->y, this->flipping);
}

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy) {
    this->x += dx;
    this->y += dy;
}