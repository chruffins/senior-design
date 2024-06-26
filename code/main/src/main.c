#include<allegro5/allegro.h>
#include<allegro5/allegro_audio.h>
#include<allegro5/allegro_acodec.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_image.h>

#include<luajit-2.1/lua.h>
#include<luajit-2.1/lualib.h>
#include<luajit-2.1/lauxlib.h>

#include "../../utils/include/utils.h"
#include "../../utils/include/allocator.h"
#include "../../utils/include/serializer.h"
#include "../../utils/include/deserializer.h"
#include "../../utils/include/globals.h"
#include "../../utils/include/drawing_thread.h"

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
    must_init(al_init_ttf_addon(), "allegro ttf library");
    must_init(al_init_primitives_addon(), "allegro primitives library");
    must_init(al_init_image_addon(), "allegro image library");

    must_init(al_install_audio(), "allegro audio");
    must_init(al_init_acodec_addon(), "allegro codec library");
    must_init(al_install_keyboard(), "allegro keyboard input");
    must_init(al_install_mouse(), "allegro mouse input");

    must_init(al_reserve_samples(16), "reserve samples");

    must_init(chrus_init(), "chrus lib");

    chrus_loader_init();

    printf("Libraries loaded correctly!\n");
}

void set_some_flags() {
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    al_inhibit_screensaver(true);
    al_set_new_window_title("chrus game");
}

void run_game_loop() {
    ALLEGRO_COND *drawing_loaded = al_create_cond();
    ALLEGRO_MUTEX *drawing_mutex = al_create_mutex();
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT event;

    chrus_scene_manager scene_manager;
    chrus_scene_manager_init(&scene_manager);

    bool finished = false;

    void *drawing_thread_args[3] = { drawing_loaded, &scene_manager, chrus_display };
    ALLEGRO_THREAD *drawing_thread = al_create_thread(drawing_handler, drawing_thread_args);
    printf("waiting until draw thread inits\n");
    al_start_thread(drawing_thread);

    al_lock_mutex(drawing_mutex);
    al_wait_cond(drawing_loaded, drawing_mutex);
    al_unlock_mutex(drawing_mutex);

    printf("drawing thread running\n");

    // al_set_target_backbuffer(chrus_display);

    al_start_timer(timer);
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(chrus_display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_set_mixer_postprocess_callback(al_get_default_mixer(), chrus_demo_postprocess_callback, NULL);

    chrus_scene_manager_load_scene(&scene_manager, "testscene.json");

    //chrus_scene_manager_add_scene(&scene_manager, chrus_scene_create("base test"));
    //chrus_node* test_node_script = malloc(sizeof(chrus_node));
    //chrus_node* datetime_script = malloc(sizeof(chrus_node));
    //chrus_node* test_image = malloc(sizeof(chrus_node));
    //*test_node_script = (chrus_node){ "script", NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/helloworld.lua") };
    //*datetime_script = (chrus_node){ "script", NULL, chrus_node_vec_create(), CHRUS_NODE_SCRIPT, chrus_script_create("data/events.lua") };
    //*test_image = (chrus_node){ "sprite", NULL, chrus_node_vec_create(), CHRUS_NODE_SPRITE, chrus_sprite_create("data/test.png")};
    
    //chrus_sprite_translate(test_image->data, 64, 64);

    //chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), test_node_script);
    //chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), datetime_script);
    //chrus_scene_add_node(chrus_scene_manager_top(&scene_manager), chrus_scene_manager_top(&scene_manager), test_image);

    while (!finished) {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            //redraw = true;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            //((chrus_camera*)(chrus_scene_manager_top(&scene_manager)->current_camera->data))->viewport_x += 5;
            break;
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            finished = true;
            break;
        default:
            break;
        }

        /*
        if (redraw) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            chrus_scene_manager_draw(&scene_manager);

            al_flip_display();
            redraw = false;
        }
        */
    }
    //chrus_audiostream_free(sound);
    //void *drawing_result;
    DEBUG_PRINTF("joined the drawing thread now\n");
    chrus_join_drawing_thread();
    al_join_thread(drawing_thread, NULL);
    al_destroy_thread(drawing_thread);
    // need to wait for drawing thread to die before we can close up the scene manager

    //printf("saving scene to disk\n");
    //chrus_serializer_save_scene(scene_manager.scenes[scene_manager.top], "savedscene.json");

    al_destroy_cond(drawing_loaded);
    al_destroy_mutex(drawing_mutex);

    DEBUG_PRINTF("freeing scene resources now\n");
    chrus_scene_manager_destroy(&scene_manager);
    DEBUG_PRINTF("freeing up cached assets now\n");
    chrus_loader_deinit();
    DEBUG_PRINTF("destroying timer now\n");
    al_destroy_timer(timer);
    DEBUG_PRINTF("destroying queue now\n");
    al_destroy_event_queue(queue);
    //printf("destroying display now\n");
    //al_destroy_display(display);
}

void finalize() {
    printf("deinitializing\n");
    chrus_deinit();
}

int main(int argc, char **argv) {
    load_allegro_libraries();

    set_some_flags();

    //chrus_deserialize_scene("savedscene.json");
    run_game_loop();

    finalize();

    return 0;
}