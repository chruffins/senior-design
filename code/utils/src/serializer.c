#include "../include/serializer.h"

static inline void fprintspaces(FILE* restrict fp, int tabs);
static inline void fprintnewfield(FILE* restrict fp, bool last);
static void fprintint(FILE* restrict fp, const char* key, int data, bool last);
static void fprintptr(FILE* restrict fp, const char* key, void* data, bool last);
static void fprintstr(FILE* restrict fp, const char* key, const char* data, bool last);
static void fprintfloat(FILE* restrict fp, const char* key, double data, bool last);
static const char *safestr(const char* str);

//static const char *chrus_node_type_to_str(enum CHRUS_NODE_TYPES);

static void serialize_and_enqueue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node_vec* children);
static void serialize_node(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node* restrict node);

static inline void serialize_camera(FILE* restrict fp, chrus_node* restrict node);
static inline void serialize_script(FILE* restrict fp, chrus_node* restrict node);
static inline void serialize_sprite(FILE* restrict fp, chrus_node* restrict node);
static inline void serialize_sound(FILE* restrict fp, chrus_node* restrict node);
static inline void serialize_audiostream(FILE* restrict fp, chrus_node* restrict node);
static inline void serialize_text(FILE* restrict fp, chrus_node* restrict node);

static inline void fprintspaces(FILE* restrict fp, int tabs) {
    fprintf(fp, "%*c", tabs * 2, ' ');
}

static inline void fprintnewfield(FILE* restrict fp, bool last) {
    if (!last) {
        fputc(',', fp);
    }
    fputc('\n', fp);
}

static void fprintint(FILE* restrict fp, const char* key, int data, bool last) {
    fprintspaces(fp, 1);
    fprintf(fp, "\"%s\": %d", key, data);
    fprintnewfield(fp, last);
}

static void fprintptr(FILE* restrict fp, const char* key, void* data, bool last) {
    fprintspaces(fp, 1);
    fprintf(fp, "\"%s\": \"%p\"", key, data);
    fprintnewfield(fp, last);
}

static void fprintstr(FILE* restrict fp, const char* key, const char* data, bool last) {
    fprintspaces(fp, 1);
    fprintf(fp, "\"%s\": \"%s\"", key, safestr(data));
    fprintnewfield(fp, last);
}

static void fprintfloat(FILE* restrict fp, const char* key, double data, bool last) {
    fprintspaces(fp, 1);
    fprintf(fp, "\"%s\": %f", key, data);
    fprintnewfield(fp, last);
}

static const char *safestr(const char* str) {
    if (str == NULL) return "";

    return str;
}

/*
static const char *chrus_node_type_to_str(enum CHRUS_NODE_TYPES type) {
    switch (type)
    {
    case CHRUS_NODE_UNINITIALIZED:
        return "uninitd";
    case CHRUS_NODE_CAMERA:
        return "camera";
    case CHRUS_NODE_SCRIPT:
        return "script";
    case CHRUS_NODE_SPRITE:
        return "sprite";
    case CHRUS_NODE_SOUND:
        return "sound";
    case CHRUS_NODE_AUDIOSTREAM:
        return "audiostream";
    }
    return "invalid";
}
*/

static void serialize_and_enqueue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node_vec* children) {
    ALLEGRO_EVENT e;
    e.user.type = 9001;

    fprintspaces(fp, 1);
    fprintf(fp, "\"children\": [\n");
    if (children->size > 0) {
        for (int i = 0; i < children->size-1; i++) {
            e.user.data1 = (intptr_t)children->data[i];
            al_emit_user_event(event_source, &e, NULL);
            fprintf(fp, "    \"%p\",\n", (void*)children->data[i]);
        }
        e.user.data1 = (intptr_t)children->data[children->size-1];
        al_emit_user_event(event_source, &e, NULL);
        fprintf(fp, "    \"%p\"\n", (void*)children->data[children->size-1]);
    }
    fprintspaces(fp, 1);
    fprintf(fp, "]\n");
}

static void serialize_node(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node* node) {
    fprintf(fp, "{\n");
    fprintint(fp, "type", node->type, false);
    fprintptr(fp, "id", node, false);
    fprintstr(fp, "name", node->name, false);
    //fprintptr(fp, "parent", node->parent, false);
    switch (node->type)
    {
    case CHRUS_NODE_UNINITIALIZED:
        /* Huh? */
        break;
    case CHRUS_NODE_CAMERA:
        serialize_camera(fp, node);
        break;
    case CHRUS_NODE_SCRIPT:
        serialize_script(fp, node);
        break;
    case CHRUS_NODE_SPRITE:
        serialize_sprite(fp, node);
        break;
    case CHRUS_NODE_SOUND:
        serialize_sound(fp, node);
        break;
    case CHRUS_NODE_AUDIOSTREAM:
        serialize_audiostream(fp, node);
        break;
    case CHRUS_NODE_TEXT:
        serialize_text(fp, node);
        break;
    default:
        break;
    }
    /* implicitly enqueues children */
    serialize_and_enqueue_children(event_source, fp, &node->children);
}

static inline void serialize_camera(FILE* restrict fp, chrus_node* restrict node) {
    chrus_camera camera = *(chrus_camera*)(node->data);

    fprintfloat(fp, "sx", camera.screen_x, false);
    fprintfloat(fp, "sy", camera.screen_y, false);
    fprintfloat(fp, "sw", camera.screen_width, false);
    fprintfloat(fp, "sh", camera.screen_height, false);
    fprintfloat(fp, "vx", camera.viewport_x, false);
    fprintfloat(fp, "vy", camera.viewport_y, false);
    fprintfloat(fp, "vw", camera.viewport_width, false);
    fprintfloat(fp, "vh", camera.viewport_height, false);
}

static inline void serialize_script(FILE* restrict fp, chrus_node* restrict node) {
    chrus_script script = *(chrus_script*)(node->data);

    fprintstr(fp, "source", script.source_name, false);
}

static inline void serialize_sprite(FILE* restrict fp, chrus_node* restrict node) {
    chrus_sprite sprite = *(chrus_sprite*)(node->data);

    fprintstr(fp, "source", sprite.source, false);
    fprintfloat(fp, "x", sprite.x, false);
    fprintfloat(fp, "y", sprite.y, false);
    // width and height are calculated values...
    fprintint(fp, "flipping", sprite.flipping, false);
    fprintfloat(fp, "sx", sprite.sx, false);
    fprintfloat(fp, "sy", sprite.sy, false);
    fprintfloat(fp, "rotation", sprite.rotation, false);
    fprintint(fp, "visible", sprite.visible, false);
}

static inline void serialize_sound(FILE* restrict fp, chrus_node* restrict node) {
    chrus_sound sound = *(chrus_sound*)(node->data);
    fprintstr(fp, "source", sound.source, false);
    // length is a calculated value fprintfloat()
    fprintint(fp, "playmode", sound.playmode, false);
    fprintfloat(fp, "gain", sound.gain, false);
    fprintfloat(fp, "pan", sound.pan, false);
    fprintfloat(fp, "speed", sound.speed, false);
}

static inline void serialize_audiostream(FILE* restrict fp, chrus_node* restrict node) {
    chrus_audiostream sound = *(chrus_audiostream*)(node->data);
    fprintstr(fp, "source", sound.source, false);
    // length is a calculated value fprintfloat()
    fprintint(fp, "playmode", sound.playmode, false);
    fprintfloat(fp, "gain", sound.gain, false);
    fprintfloat(fp, "pan", sound.pan, false);
    fprintfloat(fp, "speed", sound.speed, false);
}

static inline void serialize_text(FILE* restrict fp, chrus_node* restrict node) {
    chrus_text text = *(chrus_text*)(node->data);
    
    /* we probably need a font object... */
    /* todo: serialize color? */
    fprintfloat(fp, "x", text.x, false);
    fprintfloat(fp, "y", text.y, false);
    fprintfloat(fp, "max_width", text.max_width, false);
    fprintfloat(fp, "line_height", text.line_height, false);
    fprintint(fp, "flags", text.flags, false);
    fprintstr(fp, "text", text.text, false);
}

void chrus_serializer_save_scene(chrus_scene* scene, const char* filename) {
    ALLEGRO_EVENT_QUEUE* scene_queue = al_create_event_queue();
    ALLEGRO_EVENT_SOURCE node_saver;
    ALLEGRO_EVENT current_event;
    al_init_user_event_source(&node_saver);
    al_register_event_source(scene_queue, &node_saver);

    FILE *fp = fopen(filename, "w");

    /* need to manually save the scene node first. i know*/
    fprintf(fp, "[{\n");
    //fprintspaces(fp, 1);
    fprintint(fp, "type", -1, false);
    fprintptr(fp, "id", scene, false);
    fprintstr(fp, "name", scene->name, false);
    fprintptr(fp, "current_camera", scene->current_camera, false);
    serialize_and_enqueue_children(&node_saver, fp, &scene->children);
    fprintf(fp, "},\n");

    /* this implementation is breadth-first search :cry: */
    while (al_get_next_event(scene_queue, &current_event)) {
        chrus_node* current_node = (chrus_node*)current_event.user.data1;

        /* this function implicitly queues more nodes */
        serialize_node(&node_saver, fp, current_node);
        fputc('}', fp);
        if (!al_is_event_queue_empty(scene_queue)) {
            fputc(',', fp);
        }
        fputc('\n', fp);
    }
    fputc(']', fp);
    
    //int level = 0;
    //fprintf(fp, "{\n  \"name\": \"%s\",\n  \"current_camera\": \"%p\"}", safestr(scene->name), (void*)scene->current_camera);

    fclose(fp);
    al_destroy_user_event_source(&node_saver);
    al_destroy_event_queue(scene_queue);

    // for now, we can deserialize what we just serialized
    //chrus_serializer_deserialize_scene(filename);
}