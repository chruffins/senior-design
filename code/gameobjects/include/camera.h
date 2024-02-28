#pragma once

#include<allegro5/allegro.h>

#include<stdio.h>

#include "../../utils/include/bitmap_converter.h"

typedef struct chrus_camera_t chrus_camera;

// game objects can be seen from cameras
struct chrus_camera_t {
    // screen x and y represent "top left" of the viewport rectangle
    float screen_x, screen_y;

    // these measure in pixels how much of the camera will fill
    float screen_width, screen_height;

    // x and y represent the CENTER of the rectangle of objects rendered...
    float viewport_x, viewport_y;

    // this measures the "resolution of the camera" -- that is, how many pixels in the virtual game world are shown
    // in addition, these are "diameters"...
    float viewport_width, viewport_height;

    ALLEGRO_BITMAP* _buffer;

    // private object that transforms the view to the camera when rendering
    ALLEGRO_TRANSFORM _scaler;
};

/* this creates the data with default values, does NOT create the buffer */
chrus_camera *chrus_camera_create();

/* this creates the buffer and recalculates the transform */
void chrus_camera_init(chrus_camera* restrict this);
void chrus_camera_calc_transform(chrus_camera* restrict this);