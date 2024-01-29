#include "../include/camera.h"

chrus_camera *chrus_camera_create() {
    chrus_camera *new_camera = malloc(sizeof(chrus_camera));

    if (new_camera == NULL) {
        printf("camera failed to create!\n");
        return NULL;
    }

    new_camera->screen_x = 0;
    new_camera->screen_y = 0;

    new_camera->screen_width = 1080;
    new_camera->screen_height = 810;

    new_camera->viewport_width = 1080;
    new_camera->viewport_height = 810;

    new_camera->viewport_x = 0;
    new_camera->viewport_y = 0;

    chrus_camera_calc_transform(new_camera);

    return new_camera;
}

void chrus_camera_calc_transform(chrus_camera* this) {
    al_build_transform(&this->_scaler, 0, 0, this->screen_width / this->viewport_width, this->screen_height / this->viewport_height, 0);
}