#include "../include/sprite.h"

static chrus_sprite* chrus_sprite_create_default() {
    static ALLEGRO_BITMAP* default_square;
    if (default_square == NULL) {
        default_square = al_create_bitmap(64, 64);
        if (default_square == NULL) {
            printf("failed to create default sprite?\n");
            return NULL;
        }
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

void chrus_sprite_load(chrus_sprite* this, const char *source) {
    this->image_data = NULL;
    if (source == NULL) return;

    ALLEGRO_BITMAP *loader = chrus_loader_insert(CHRUS_LOADER_BITMAP, source);
    if (!loader) return;

    this->image_data = loader;
}

chrus_sprite* chrus_sprite_create(const char *source) {
    if (source == NULL) return chrus_sprite_create_default();

    ALLEGRO_BITMAP *loader = chrus_loader_insert(CHRUS_LOADER_BITMAP, source);
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

void chrus_sprite_draw(chrus_sprite *this, float dx, float dy) {
    al_draw_bitmap(this->image_data, this->x + dx, this->y + dy, this->flipping);
}

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy) {
    this->x += dx;
    this->y += dy;
}