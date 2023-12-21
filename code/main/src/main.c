#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>

#include<luajit/lua.h>
#include<luajit/lualib.h>
#include<luajit/lauxlib.h>

#include "../../utils/include/utils.h"

#include "../../gameobjects/include/scenemanager.h"
#include "../../gameobjects/include/node.h"
#include "../../gameobjects/include/script.h"
#include "../../gameobjects/include/chrus.h"

#include<stdbool.h>
#include<stdio.h>

// this function prints and exits with an error code if the libs don't load
void must_init(int test, const char *failure) {
    if (test) return;

    printf("%s failed to load!", failure);
    exit(ERROR_FAILED_TO_INIT_LIB);
}

// if any of the loads fail, the must_inits will shit the bed and exit the program prematurely
void load_allegro_libraries() {
    must_init(al_init(), "allegro");
    must_init(al_init_font_addon(), "allegro font library");
    must_init(al_init_primitives_addon(), "allegro primitives library");
    must_init(al_init_image_addon(), "allegro image library");

    must_init(al_install_audio(), "allegro audio");
    must_init(al_init_acodec_addon(), "allegro codec library");
    must_init(al_install_keyboard(), "allegro keyboard input");
    must_init(al_install_mouse(), "allegro mouse input");

    must_init(al_reserve_samples(16), "reserve samples");

    must_init(chrus_init(), "chrus lib");

    printf("Libraries loaded correctly!\n");
}

void set_some_flags() {
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    al_set_new_window_title("fuckwad");
}

void run_game_loop() {
    ALLEGRO_DISPLAY *display = al_create_display(1080, 810);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT event;

    chrus_scene_manager scene_manager;
    chrus_scene_manager_init(&scene_manager);

    bool finished = false;
    bool redraw = false;

    al_start_timer(timer);
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    chrus_scene_manager_add_scene(&scene_manager, chrus_scene_create("base test"));
    chrus_node* test_node_script = malloc(sizeof(chrus_node));
    chrus_node* datetime_script = malloc(sizeof(chrus_node));
    chrus_node* test_image = malloc(sizeof(chrus_node));
    *test_node_script = (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/helloworld.lua") };
    *datetime_script = (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/datetime.lua") };
    *test_image = (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SPRITE, chrus_sprite_create("data/test.png")};

    //chrus_sound* sound = chrus_sound_create("data/fruitcake.flac");
    //chrus_sound_play(sound);
    
    chrus_sprite_translate(test_image->data, 64, 64);

    /*
    // a thousand test pngs descend on you!
    for (int i = 0; i < 1000; i++) {
        chrus_node *image = malloc(sizeof(chrus_node));
        *image = (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SPRITE, chrus_sprite_create("data/test.png") };
        chrus_sprite_translate(image->data, rand() % 1080, rand() % 810);
        ((chrus_sprite*)(image->data))->flipping = rand() % 2 * ALLEGRO_FLIP_HORIZONTAL;
        chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), image);
    }
    */

    //chrus_node_vec_add_node(&scene_manager.scenes[scene_manager.top]->children, (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/helloworld.lua") });
    chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), test_node_script);
    chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), datetime_script);
    chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), test_image);
    //chrus_node node = (chrus_node){ NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/helloworld.lua") };

    while (!finished) {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            ((chrus_camera*)(chrus_scene_manager_top(&scene_manager)->current_camera->data))->viewport_x += 5;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            finished = true;
            break;
        default:
            break;
        }

        if (redraw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            chrus_scene_manager_draw(&scene_manager);

            al_flip_display();
            redraw = false;
        }
    }
    //chrus_sound_free(sound);

    printf("freeing scene resources now\n");
    chrus_scene_manager_destroy(&scene_manager);
    printf("destroying timer now\n");
    al_destroy_timer(timer);
    printf("destroying queue now\n");
    al_destroy_event_queue(queue);
    printf("destroying display now\n");
    al_destroy_display(display);
}

void finalize() {
    printf("deinitializing\n");
    chrus_deinit();
}

int main(int argc, char **argv) {
    load_allegro_libraries();

    set_some_flags();

    run_game_loop();

    finalize();

    return 0;
}