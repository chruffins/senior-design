#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>

#include "../../luajit/src/lua.h"

#include "../../utils/include/utils.h"

#include<stdbool.h>
#include<stdio.h>

// this function prints and exits with an error code if the libs don't load
void try_init(int test, const char *failure) {
    if (test) return;

    printf("%s failed to load!", failure);
    exit(ERROR_FAILED_TO_INIT_LIB);
}

// if any of the loads fail, the try_inits will shit the bed and exit the program prematurely
void load_allegro_libraries() {
    try_init(al_init(), "allegro");
    try_init(al_init_font_addon(), "allegro font library");
    try_init(al_init_primitives_addon(), "allegro primitives library");
    try_init(al_init_image_addon(), "allegro image library");

    try_init(al_install_audio(), "allegro audio");
    try_init(al_install_keyboard(), "allegro keyboard input");
    try_init(al_install_mouse(), "allegro mouse input");

    printf("Libraries loaded correctly!\n");
}

int main(int argc, char **argv) {
    load_allegro_libraries();

    return 0;
}