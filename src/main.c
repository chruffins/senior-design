#include<allegro5/allegro.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>
#include<allegro5/allegro_primitives.h>
#include<stdio.h>

#include "gui.h"

int main(int argc, char **argv) {
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT event;

    display = al_create_display(640, 480);
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    
    gui_element test_label = gui_label_create();

    test_label.draw(&test_label);
    al_flip_display();

    while (1) {
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }
    printf("Hello world!\n");
}