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
    fprintint(fp, "type", node->type, false);
    fprintptr(fp, "id", node, false);
    fprintstr(fp, "name", node->name, false);
    fprintptr(fp, "parent", node->parent, false);
    /* implicitly enqueues children */
    serialize_and_enqueue_children(event_source, fp, &node->children);
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
        chrus_node* current_node = current_event.user.data1;

        /* this function implicitly queues more nodes */
        serialize_node(&scene_queue, fp, current_node);
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
    chrus_serializer_deserialize_scene(filename);
}

static int get_key_and_value_type(json_stream* stream, const char* keybuffer, size_t* keylength, enum json_type* valuetype) {
    *valuetype = json_next(&stream);
    if (*valuetype == JSON_OBJECT_END) return CHRUS_PARSER_OBJECT_ENDED;
    if (*valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;
    
    keybuffer = json_get_string(&stream, keylength);
    *valuetype = json_next(&stream);

    return 0;
}

/* deserialization:
    first pass through json: allocate ALL of the nodes
    second pass through json: create the connections!!
*/
chrus_scene* chrus_serializer_deserialize_scene(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    printf("successfully opened scene file\n");

    json_stream first_pass_stream;
    enum json_type current_type;
    chrus_scene* final_scene = NULL;
    chrus_vector nodes = chrus_vector_create();

    size_t field_length;
    const char* str_buffer;
    double number_buffer;

    json_open_stream(&first_pass_stream, fp);

    // let's make sure we're reading a json array then a json object
    // we need to do some assertions too...
    current_type = json_next(&first_pass_stream);
    if (current_type != JSON_ARRAY) goto failure;
    /* reading the scene object now */
    current_type = json_next(&first_pass_stream);
    if (current_type != JSON_OBJECT) goto failure;
    current_type = json_next(&first_pass_stream);
    if (current_type != JSON_STRING) goto failure;

    while (get_key_and_value_type(&first_pass_stream, str_buffer, &field_length, &current_type) == 0) {
        
    }
    str_buffer = json_get_string(&first_pass_stream, &field_length);
    printf("scene field: %s\n", str_buffer);
    current_type = json_next(&first_pass_stream);
    number_buffer = json_get_number(&first_pass_stream);
    printf("scene type: %d\n", (int)number_buffer);
    goto success;

    failure:
    printf("experienced a failure\n");
    if (final_scene) chrus_scene_destroy(final_scene);
    return NULL;
    success:
    return final_scene;
}