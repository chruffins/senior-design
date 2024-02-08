#include "../include/serializer.h"

static inline void fprintspaces(FILE* restrict fp, int tabs);
static inline void fprintnewfield(FILE* restrict fp, bool last);
static void fprintint(FILE* restrict fp, const char* key, int data, bool last);
static void fprintptr(FILE* restrict fp, const char* key, void* data, bool last);
static void fprintstr(FILE* restrict fp, const char* key, const char* data, bool last);
static void fprintfloat(FILE* restrict fp, const char* key, double data, bool last);
static const char *safestr(const char* str);

static const char *chrus_node_type_to_str(enum CHRUS_NODE_TYPES);

static void serialize_and_enqueue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node_vec* children);
static void serialize_node(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node* restrict node);
static void queue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, chrus_node_vec* restrict children);

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
    fprintf(fp, "\"%s\": \"%d\"", key, data);
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
    fprintf(fp, "\"%s\": \"%f\"", key, data);
    fprintnewfield(fp, last);
}

static const char *safestr(const char* str) {
    if (str == NULL) return "";

    return str;
}

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

static void serialize_and_enqueue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node_vec* children) {
    ALLEGRO_EVENT e;
    e.user.type = 9001;

    fprintspaces(fp, 1);
    fprintf(fp, "\"children\": [\n");
    if (children->size > 0) {
        for (int i = 0; i < children->size-1; i++) {
            printf("%p\n", children->data[i]);
            e.user.data1 = children->data[i];
            al_emit_user_event(event_source, &e, NULL);
            fprintf(fp, "    \"%p\",\n", children->data[i]);
        }
        e.user.data1 = children->data[children->size-1];
        al_emit_user_event(event_source, &e, NULL);
        fprintf(fp, "    \"%p\"\n");
    }
    fprintspaces(fp, 1);
    fprintf(fp, "]\n");
}

static void serialize_node(ALLEGRO_EVENT_SOURCE* restrict event_source, FILE* restrict fp, chrus_node* node) {
    fprintf(fp, "{\n");
    fprintptr(fp, "id", node, false);
    fprintstr(fp, "name", node->name, false);
    fprintstr(fp, "type", chrus_node_type_to_str(node->type), false);
    fprintptr(fp, "parent", node->parent, false);
    /* implicitly enqueues children */
    serialize_and_enqueue_children(event_source, fp, &node->children);
    fputs("},\n", fp);
}

static void queue_children(ALLEGRO_EVENT_SOURCE* restrict event_source, chrus_node_vec* restrict children) {
    ALLEGRO_EVENT e;
    e.user.type = 9001; // really doesn't matter
    
    for (size_t i = 0; i < children->size; i++) {
        e.user.data1 = children->data[i];

        al_emit_user_event(event_source, &e, NULL);
    }
}

void chrus_serializer_save_scene(chrus_scene* scene, const char* filename) {
    ALLEGRO_EVENT_QUEUE* scene_queue = al_create_event_queue();
    ALLEGRO_EVENT_SOURCE node_saver;
    ALLEGRO_EVENT current_event;
    al_init_user_event_source(&node_saver);
    al_register_event_source(scene_queue, &node_saver);

    FILE *fp = fopen(filename, "w");

    /* need to manually save the scene node first. i know*/
    fprintf(fp, "{\n");
    //fprintspaces(fp, 1);
    fprintptr(fp, "id", scene, false);
    fprintstr(fp, "name", scene->name, false);
    fprintptr(fp, "current_camera", scene->current_camera, false);
    serialize_and_enqueue_children(&node_saver, fp, &scene->children);
    fprintf(fp, "},\n");

    /* this implementation is breadth-first search :cry: */
    while (al_get_next_event(scene_queue, &current_event)) {
        chrus_node* current_node = current_event.user.data1;

        /* this function implicitly queues more nodes */
        serialize_node(&scene_queue, fp, current_node);
    }
    
    //int level = 0;
    //fprintf(fp, "{\n  \"name\": \"%s\",\n  \"current_camera\": \"%p\"}", safestr(scene->name), (void*)scene->current_camera);

    fclose(fp);
    al_destroy_user_event_source(&node_saver);
    al_destroy_event_queue(scene_queue);
}