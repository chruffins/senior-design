#include "../include/drawing_thread.h"

void* drawing_handler(ALLEGRO_THREAD *this, void *args) {
    void** pargs = (void**)args;
    ALLEGRO_COND* drawing_cond = (ALLEGRO_COND*)pargs[0];
    chrus_scene_manager* scene_manager = (chrus_scene_manager*)pargs[1];
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); // separate queue that only takes in drawing events

    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
    //al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE);
    chrus_display = al_create_display(1080, 810);

    /* need to setup the drawing queue and the ability to send over bitmaps to create */
    chrus_drawing_queue = al_create_event_queue();
    al_init_user_event_source(&chrus_drawing_event_source);

    chrus_sprite_create(NULL); /* you did this to initialize the default sprite */
    if (!chrus_load_default_font()) {
        printf("default font FAILED to load in! you will crash now lol\n");
    } else {
        printf("default font loaded in :)\n");
    }
    
    ALLEGRO_TIMER* draw_timer = al_create_timer(1.0 / 60.0);
    //ALLEGRO_DISPLAY *display = (ALLEGRO_DISPLAY*)pargs[2];
    ALLEGRO_EVENT event;

    double fps = 0;
    uint64_t frames_done = 0;
    double old_time = al_get_time();

    ALLEGRO_FONT* def = al_create_builtin_font();

    DEBUG_PRINTF("ready to start main thread again\n");

    al_broadcast_cond(drawing_cond);

    al_start_timer(draw_timer);
    al_register_event_source(queue, al_get_timer_event_source(draw_timer));
    al_register_event_source(queue, &chrus_drawing_event_source);

    bool redraw = false;
    bool finished = false;
    while (1) {
        al_wait_for_event(queue, &event);
        switch (event.type)
        {
        case CHRUS_EVENT_JOIN_DRAWING:
            finished = true;
            break;
        case CHRUS_EVENT_CONVERT_BITMAP:
            al_convert_memory_bitmaps();
            //al_convert_bitmap((ALLEGRO_BITMAP*)event.user.data1);
            break;
        case CHRUS_EVENT_CREATE_SHADER:
            *(void**)event.user.data1 = al_create_shader(ALLEGRO_SHADER_GLSL);
            al_broadcast_cond((ALLEGRO_COND*)event.user.data2);
            break;
        case CHRUS_EVENT_ATTACH_SOURCEFILE_SHADER:
            al_attach_shader_source_file((ALLEGRO_SHADER*)event.user.data1, (ALLEGRO_SHADER_TYPE)event.user.data2, (const char*)event.user.data3);
            break;
        case CHRUS_EVENT_ATTACH_SOURCE_SHADER:
            al_attach_shader_source((ALLEGRO_SHADER*)event.user.data1, (ALLEGRO_SHADER_TYPE)event.user.data2, (const char*)event.user.data3);
            break;
        case CHRUS_EVENT_BUILD_SHADER:
            al_build_shader((ALLEGRO_SHADER*)event.user.data1);
            break;
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;
        }

        if (finished) break;
        if (!redraw || !al_is_event_queue_empty(queue)) continue;

        al_clear_to_color(al_map_rgb(0, 0, 0));

        chrus_scene_manager_draw(scene_manager);
        // we'll probably return some kind of error code in the future in case there's a catastrophic error with this
        al_draw_textf(def, al_map_rgb(255, 0, 0), 0, 0, 0, "%.0f FPS", fps);

        al_flip_display();

        double game_time = al_get_time();
        if (game_time - old_time >= 1.0) {
            fps = frames_done / (game_time - old_time);

            frames_done = 0;
            old_time = game_time;
        }  

        frames_done++;
        redraw = false;
    }
    al_stop_timer(draw_timer);
    al_destroy_timer(draw_timer);
    al_destroy_event_queue(queue);

    return NULL;
}