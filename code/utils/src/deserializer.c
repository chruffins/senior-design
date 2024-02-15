#include "../include/deserializer.h"

#define READ_JSON_NUMBER(stream, jfieldname, fieldname) if (is_correct_fieldname(stream, jfieldname) || is_correct_type(stream, JSON_NUMBER)) { \
        goto cleanup; \
    }   else {\
        fieldname = json_get_number(stream);\
    }

#define READ_JSON_NUMBER_WITH_CAST(stream, jfieldname, fieldname, cast_type) if (is_correct_fieldname(stream, jfieldname) || is_correct_type(stream, JSON_NUMBER)) { \
        goto cleanup; \
    }   else {\
        fieldname = (cast_type)json_get_number(stream);\
    }

#define READ_JSON_STRING(stream, jfieldname, fieldname) if (is_correct_fieldname(stream, jfieldname) || is_correct_type(stream, JSON_STRING)) { \
        goto cleanup; \
    }   else {\
        fieldname = json_get_string(stream, NULL);\
    }
/* deserialization:
    first pass through json: allocate ALL of the nodes
    second pass through json: create the connections!!
*/

/* NOT A PURE FUNCTION!! THIS ADVANCES THE STREAM TO CHECK CORRECTNESS */
static inline int is_correct_fieldname(json_stream* stream, const char* fieldname);

/* NOT A PURE FUNCTION!! THIS ADVANCES THE STREAM TO CHECK CORRECTNESS */
static inline int is_correct_type(json_stream* stream, enum json_type jtype);

static inline int get_number_field(json_stream* stream, const char* fieldname, double* number_buffer);
static inline int get_string_field(json_stream* stream, const char* fieldname, const char** string_buffer);

/* chrus_rbtree* restrict tree, */
static inline chrus_node* deserialize_object(chrus_rbtree* restrict tree, json_stream* restrict stream);
static inline chrus_node* deserialize_camera(json_stream* stream);
static inline chrus_node* deserialize_script(json_stream* stream);
static inline chrus_node* deserialize_sprite(json_stream* stream);
static inline chrus_node* deserialize_sound(json_stream* stream);
static inline chrus_node* deserialize_audiostream(json_stream* stream);
static inline chrus_node* deserialize_text(json_stream* stream);

static inline int is_correct_fieldname(json_stream* stream, const char* fieldname) {
    enum json_type valuetype;
    const char* fieldbuffer;
    valuetype = json_next(stream);
    if (valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;

    fieldbuffer = json_get_string(stream, NULL);
    if (strcmp(fieldbuffer, fieldname) == 0) {
        free(fieldbuffer);
        return 0;
    } else {
        free(fieldbuffer);
        return 1;
    }
}

static inline int is_correct_type(json_stream* stream, enum json_type jtype) {
    if (json_next(stream) == jtype) {
        return 0;
    } else {
        return 1;
    }
}

static inline int get_number_field(json_stream* stream, const char* fieldname, double* number_buffer) {
    enum json_type valuetype;
    valuetype = json_next(stream);
    if (valuetype == JSON_OBJECT_END) return CHRUS_PARSER_OBJECT_ENDED;
    if (valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;

    *number_buffer = json_get_number(stream);
}

static int get_key_and_value_type(json_stream* stream, const char* keybuffer, size_t* keylength, enum json_type* valuetype) {
    *valuetype = json_next(&stream);
    if (*valuetype == JSON_OBJECT_END) return CHRUS_PARSER_OBJECT_ENDED;
    if (*valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;
    
    keybuffer = json_get_string(&stream, keylength);
    *valuetype = json_next(&stream);

    return 0;
}

static inline chrus_node* deserialize_camera(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_camera* camera = chrus_camera_create();

    node->data = camera;

    READ_JSON_NUMBER(stream, "sx", camera->screen_x)
    READ_JSON_NUMBER(stream, "sy", camera->screen_y)
    READ_JSON_NUMBER(stream, "sw", camera->screen_width)
    READ_JSON_NUMBER(stream, "sh", camera->screen_height)
    READ_JSON_NUMBER(stream, "vx", camera->viewport_x)
    READ_JSON_NUMBER(stream, "vy", camera->viewport_y)
    READ_JSON_NUMBER(stream, "vw", camera->viewport_width)
    READ_JSON_NUMBER(stream, "vh", camera->viewport_height)

    return node;

    cleanup:
    chrus_node_destroy(node); /* implicitly destroys camera */
    return NULL;
}

static inline chrus_node* deserialize_script(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_script* script = chrus_script_create(NULL);

    node->data = script;

    READ_JSON_STRING(stream, "source", script->source_name)

    return node;

    cleanup:
    chrus_node_destroy(node);
    return NULL;
}

static inline chrus_node* deserialize_sprite(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_sprite* sprite = chrus_sprite_create(NULL);

    node->data = sprite;

    READ_JSON_STRING(stream, "source", sprite->source)
    READ_JSON_NUMBER(stream, "x", sprite->x)
    READ_JSON_NUMBER(stream, "y", sprite->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "flipping", sprite->x, int)
    READ_JSON_NUMBER(stream, "sx", sprite->x)
    READ_JSON_NUMBER(stream, "sy", sprite->x)
    READ_JSON_NUMBER(stream, "rotation", sprite->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "visible", sprite->x, bool)

    return node;
    cleanup:
    chrus_node_destroy(node);
    return NULL;
}

static inline chrus_node* deserialize_sound(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_sound* sound = chrus_sound_create(NULL);

    node->data = sound;

    READ_JSON_STRING(stream, "source", sound->source)
    READ_JSON_NUMBER_WITH_CAST(stream, "playmode", sound->playmode, int)
    READ_JSON_NUMBER(stream, "gain", sound->gain)
    READ_JSON_NUMBER(stream, "pan", sound->pan)
    READ_JSON_NUMBER(stream, "speed", sound->speed)

    return node;
    cleanup:
    chrus_node_destroy(node);
    return NULL;
}

static inline chrus_node* deserialize_audiostream(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_sound* sound = chrus_audiostream_create(NULL);

    node->data = sound;

    READ_JSON_STRING(stream, "source", sound->source)
    READ_JSON_NUMBER_WITH_CAST(stream, "playmode", sound->playmode, int)
    READ_JSON_NUMBER(stream, "gain", sound->gain)
    READ_JSON_NUMBER(stream, "pan", sound->pan)
    READ_JSON_NUMBER(stream, "speed", sound->speed)

    return node;
    cleanup:
    chrus_node_destroy(node);
    return NULL;
}

static inline chrus_node* deserialize_text(json_stream* stream) {
    chrus_node* node = chrus_node_create_uninit();
    chrus_text* text = chrus_text_create();

    node->data = text;

    READ_JSON_NUMBER(stream, "x", text->x)
    READ_JSON_NUMBER(stream, "y", text->x)
    READ_JSON_NUMBER(stream, "max_width", text->x)
    READ_JSON_NUMBER(stream, "line_height", text->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "x", text->flags, int)

    return node;
    cleanup:
    chrus_node_destroy(node);
    return NULL;
}

/*
    CHRUS_NODE_CAMERA,
    CHRUS_NODE_SCRIPT,
    CHRUS_NODE_SPRITE,
    CHRUS_NODE_SOUND,
    CHRUS_NODE_AUDIOSTREAM,
    CHRUS_NODE_TEXT,
*/
static inline chrus_node* deserialize_object(chrus_rbtree* restrict tree, json_stream* restrict stream) {
    enum json_type current_type;
    
    chrus_node* final_node = NULL;

    /* collect this from the first three fields */
    enum CHRUS_NODE_TYPES node_type;
    const char* node_name;
    uint64_t* node_ptr = malloc(sizeof(uint64_t));

    /* TODO: figure out what to do if we error */
    current_type = json_next(stream);
    if (current_type != JSON_OBJECT) return NULL;

    is_correct_fieldname(stream, "type");
    current_type = json_next(stream);
    if (current_type != JSON_NUMBER) return NULL;
    node_type = (int)json_get_number(stream);
    final_node->type = node_type; /* set the node type now */

    is_correct_fieldname(stream, "id");
    current_type = json_next(stream);
    if (current_type != JSON_STRING) return NULL;
    /* extracting hexstring into uint64 so that we can use it as a key */
    const char* hexbuffer = json_get_string(stream, NULL);
    sscanf(hexbuffer, "%"SCNx64"", node_ptr);
    free(hexbuffer);

    is_correct_fieldname(stream, "name");
    current_type = json_next(stream);
    if (current_type != JSON_STRING) return NULL;
    node_name = json_get_string(stream, NULL);
    /* TODO: string interning? but then garbage collection... */
    final_node->name = node_name;

    switch (node_type)
    {
    case CHRUS_NODE_UNINITIALIZED:
        /* Huh? */
        break;
    case CHRUS_NODE_CAMERA:
        final_node = deserialize_camera(stream);
        break;
    case CHRUS_NODE_SCRIPT:
        final_node = deserialize_script(stream);
        break;
    case CHRUS_NODE_SPRITE:
        final_node = deserialize_sprite(stream);
        break;
    case CHRUS_NODE_SOUND:
        final_node = deserialize_sound(stream);
        break;
    case CHRUS_NODE_AUDIOSTREAM:
        final_node = deserialize_audiostream(stream);
        break;
    case CHRUS_NODE_TEXT:
        final_node = deserialize_text(stream);
        break;
    default:
        break;
    }

    if (final_node == NULL) return NULL;

    /* building our treemap now */
    chrus_rbtree_insert_pair(tree, node_ptr, final_node);
    return final_node;
}

chrus_scene* chrus_serializer_deserialize_scene(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    printf("successfully opened scene file\n");

    json_stream first_pass_stream;
    enum json_type current_type;
    chrus_scene* final_scene = NULL;
    chrus_rbtree* pointer_dictionary = chrus_rbtree_create(NULL, NULL, NULL);
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