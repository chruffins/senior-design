#include "../include/deserializer.h"

/* deserialization:
    first pass through json: allocate ALL of the nodes
    second pass through json: create the connections!!
*/

/* you pass fieldname to assert the field name is correct, and number buffer to get the actual value*/
static inline int assert_fieldname(json_stream* stream, const char* fieldname);
static inline int get_number_field(json_stream* stream, const char* fieldname, double* number_buffer);
static inline int get_string_field(json_stream* stream, const char* fieldname, const char** string_buffer);

static inline chrus_node* deserialize_object(json_stream* stream);
static inline chrus_node* deserialize_camera(json_stream* stream);
static inline chrus_node* deserialize_script(json_stream* stream);
static inline chrus_node* deserialize_sprite(json_stream* stream);
static inline chrus_node* deserialize_sound(json_stream* stream);
static inline chrus_node* deserialize_audiostream(json_stream* stream);
static inline chrus_node* deserialize_text(json_stream* stream);

static inline int assert_fieldname(json_stream* stream, const char* fieldname) {
    enum json_type valuetype;
    const char* fieldbuffer;
    valuetype = json_next(&stream);
    if (valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;

    fieldbuffer = json_get_string(&stream, NULL);
    if (strcmp(fieldbuffer, fieldname) == 0) {
        free(fieldbuffer);
        return 0;
    } else {
        free(fieldbuffer);
        return 1;
    }
}

static inline int get_number_field(json_stream* stream, const char* fieldname, double* number_buffer) {
    enum json_type valuetype;
    valuetype = json_next(&stream);
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

}

static inline chrus_node* deserialize_script(json_stream* stream) {

}

static inline chrus_node* deserialize_sprite(json_stream* stream) {

}

static inline chrus_node* deserialize_sound(json_stream* stream) {

}

static inline chrus_node* deserialize_audiostream(json_stream* stream) {

}

static inline chrus_node* deserialize_text(json_stream* stream) {
    
}

/*
    CHRUS_NODE_CAMERA,
    CHRUS_NODE_SCRIPT,
    CHRUS_NODE_SPRITE,
    CHRUS_NODE_SOUND,
    CHRUS_NODE_AUDIOSTREAM,
    CHRUS_NODE_TEXT,
*/
static inline chrus_node* deserialize_object(json_stream* stream) {
    enum json_type current_type;
    enum CHRUS_NODE_TYPES node_type;
    chrus_node* final_node = NULL;

    /* TODO: figure out what to do if we error */
    current_type = json_next(&stream);
    if (current_type != JSON_OBJECT) {}
    assert_fieldname(stream, "type");

    current_type = json_next(&stream);
    if (current_type != JSON_NUMBER) {} 
    node_type = (int)json_get_number(stream);

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
}

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