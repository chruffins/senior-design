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
    new_sprite->source = NULL;
    new_sprite->x = 0;
    new_sprite->y = 0;
    new_sprite->width = 64;
    new_sprite->height = 64;
    new_sprite->rotation = 0;
    new_sprite->flipping = 0;
    new_sprite->sx = 1;
    new_sprite->sy = 1;
    new_sprite->visible = true;
    new_sprite->_layer = 0;
    new_sprite->image_data = default_square;

    return new_sprite;
}

void chrus_sprite_load(chrus_sprite* this, const char *source) {
    this->source = source;
    this->image_data = NULL;
    if (source == NULL) return;

    ALLEGRO_BITMAP *loader = chrus_loader_insert(CHRUS_LOADER_BITMAP, chrus_rbkey_create(source));
    if (!loader) {
        printf("sprite failed to load\n");
        return;
    }

    this->image_data = loader;
    this->width = al_get_bitmap_width(this->image_data);
    this->height = al_get_bitmap_height(this->image_data);
}

chrus_sprite* chrus_sprite_create_uninit() {
    chrus_sprite *new_sprite = calloc(1, sizeof(chrus_sprite));

    return new_sprite;
}

chrus_sprite* chrus_sprite_create(const char *source) {
    if (source == NULL) return chrus_sprite_create_default();

    ALLEGRO_BITMAP *loader = chrus_loader_insert(CHRUS_LOADER_BITMAP, chrus_rbkey_create(source));
    if (!loader) return chrus_sprite_create_default();

    chrus_sprite *new_sprite = malloc(sizeof(chrus_sprite));

    new_sprite->x = 0;
    new_sprite->y = 0;
    new_sprite->width = al_get_bitmap_width(loader);
    new_sprite->height = al_get_bitmap_height(loader);
    new_sprite->sx = 1;
    new_sprite->sy = 1;
    new_sprite->rotation = 0;
    new_sprite->flipping = 0;
    new_sprite->visible = true;
    new_sprite->image_data = loader;
    new_sprite->_layer = 0;

    return new_sprite;
}

void chrus_sprite_draw(chrus_sprite *this, float dx, float dy) {
    if (!this->visible) return;
    //al_draw_rotated_bitmap(this->image_data, (this->width / 2), (this->height / 2), this->x + dx, this->y + dy, this->rotation, this->flipping);
    //al_draw_bitmap(this->image_data, this->x + dx, this->y + dy, this->flipping);
    al_draw_scaled_rotated_bitmap(this->image_data, 0, 0, this->x + dx, this->y + dy, this->sx, this->sy, this->rotation, this->flipping);
}

void chrus_sprite_translate(chrus_sprite *this, float dx, float dy) {
    this->x += dx;
    this->y += dy;
}

void chrus_sprite_destroy(chrus_sprite* restrict this) {
    free(this);
}

float chrus_sprite_get_x(chrus_sprite* restrict this) {
    return this->x;
}

float chrus_sprite_get_y(chrus_sprite* restrict this) {
    return this->y;
}

int chrus_sprite_get_width(chrus_sprite* restrict this) {
    return this->width;
}

int chrus_sprite_get_height(chrus_sprite* restrict this) {
    return this->height;
}

float chrus_sprite_get_sx(chrus_sprite* restrict this) {
    return this->sx;
}

float chrus_sprite_get_sy(chrus_sprite* restrict this) {
    return this->sy;
}

int chrus_sprite_get_flipping(chrus_sprite* restrict this) {
    return this->flipping;
}

float chrus_sprite_get_rotation(chrus_sprite* restrict this) {
    return this->rotation;
}

bool chrus_sprite_get_visible(chrus_sprite* restrict this) {
    return this->visible;
}

int chrus_sprite_get_layer(chrus_sprite* restrict this) {
    return this->_layer;
}

ALLEGRO_BITMAP* chrus_sprite_get_bitmap(chrus_sprite* restrict this) {
    return this->image_data;
}

void chrus_sprite_set_x(chrus_sprite* restrict this, float new) {
    this->x = new;
}

void chrus_sprite_set_y(chrus_sprite* restrict this, float new) {
    this->y = new;
}

void chrus_sprite_set_sx(chrus_sprite* restrict this, float new) {
    this->sx = new;
}

void chrus_sprite_set_sy(chrus_sprite* restrict this, float new) {
    this->sy = new;
}

void chrus_sprite_set_flipping(chrus_sprite* restrict this, int new) {
    this->flipping = new;
}

void chrus_sprite_set_rotation(chrus_sprite* restrict this, float new) {
    this->rotation = new;
}

void chrus_sprite_set_visible(chrus_sprite* restrict this, bool new) {
    this->visible = new;
}

void chrus_sprite_set_bitmap(chrus_sprite* restrict this, ALLEGRO_BITMAP* restrict new) {
    this->image_data = new;
    this->width = al_get_bitmap_width(new);
    this->height = al_get_bitmap_height(new);
}