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

void set_some_flags() {
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_window_title("fuckwad");
}

void run_game_loop() {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT event;

    bool finished = false;
    bool redraw = false;

    display = al_create_display(1080, 810);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    while (!finished) {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            finished = true;
            break;
        default:
            break;
        }

        if (redraw) {
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_display(display);
    al_destroy_event_queue(queue);
}

int main(int argc, char **argv) {
    load_allegro_libraries();

    set_some_flags();

    run_game_loop();

    return 0;
}