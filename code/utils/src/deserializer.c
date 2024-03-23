#include "../include/deserializer.h"

#define READ_JSON_NUMBER(stream, jfieldname, fieldname) if (is_correct_fieldname(stream, jfieldname) != 0 || is_correct_type(stream, JSON_NUMBER) != 0) { \
        goto cleanup; \
    }   else {\
        fieldname = json_get_number(stream);\
    }

#define READ_JSON_NUMBER_WITH_CAST(stream, jfieldname, fieldname, cast_type) if (is_correct_fieldname(stream, jfieldname) != 0 || is_correct_type(stream, JSON_NUMBER) != 0) { \
        goto cleanup; \
    }   else {\
        fieldname = (cast_type)json_get_number(stream);\
    }

#define READ_JSON_STRING(stream, jfieldname, fieldname) if (is_correct_fieldname(stream, jfieldname) != 0 || is_correct_type(stream, JSON_STRING) != 0) { \
        goto cleanup; \
    }   else {\
        fieldname = json_get_string_copy(stream);\
    }

#define READ_JSON_COLOR(stream, jfieldname, fieldname) if (is_correct_fieldname(stream, jfieldname) != 0 || is_correct_type(stream, JSON_STRING) != 0) { \
        goto cleanup; \
    }   else {\
        uint32_t temp_value = strtoul(json_get_string(stream, NULL) + 1, NULL, 16);\
        fieldname = al_map_rgba((temp_value) >> 24 & 0xFF, (temp_value >> 16) & 0xFF, (temp_value) >> 8 & 0xFF, temp_value & 0xFF);\
    }
/* deserialization:
    first pass through json: allocate ALL of the nodes
    second pass through json: create the connections!!
*/

/* NOT A PURE FUNCTION!! THIS ADVANCES THE STREAM TO CHECK CORRECTNESS */
static inline int is_correct_fieldname(json_stream* stream, const char* fieldname);

/* NOT A PURE FUNCTION!! THIS ADVANCES THE STREAM TO CHECK CORRECTNESS */
static inline int is_correct_type(json_stream* stream, enum json_type jtype);

/* chrus_rbtree* restrict tree, */
static inline chrus_node* deserialize_object(chrus_rbtree* restrict tree, json_stream* restrict stream);
static inline chrus_node* deserialize_camera(json_stream* stream);
static inline chrus_node* deserialize_script(json_stream* stream);
static inline chrus_node* deserialize_sprite(json_stream* stream);
static inline chrus_node* deserialize_sound(json_stream* stream);
static inline chrus_node* deserialize_audiostream(json_stream* stream);
static inline chrus_node* deserialize_text(json_stream* stream);

/* your responsibility to free the string */
static const char* json_get_string_copy(json_stream* stream);

/* advances through entire stream, returns children stored */
static inline int store_children_in_rbtree(chrus_vector* restrict vec, chrus_rbtree* restrict tree, json_stream* restrict stream);

/* advances through entire stream */
static inline int count_children(json_stream* restrict stream);

static inline int is_correct_fieldname(json_stream* stream, const char* fieldname) {
    enum json_type valuetype;
    const char* fieldbuffer;
    valuetype = json_next(stream);
    if (valuetype != JSON_STRING) return CHRUS_PARSER_ERROR;

    fieldbuffer = json_get_string(stream, NULL);
    if (fieldbuffer && strcmp(fieldbuffer, fieldname) == 0) {
        return 0;
    } else {
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
    chrus_sprite* sprite = chrus_sprite_create_uninit();

    node->data = sprite;

    READ_JSON_STRING(stream, "source", sprite->source)
    READ_JSON_NUMBER(stream, "x", sprite->x)
    READ_JSON_NUMBER(stream, "y", sprite->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "flipping", sprite->x, int)
    READ_JSON_NUMBER(stream, "sx", sprite->x)
    READ_JSON_NUMBER(stream, "sy", sprite->x)
    READ_JSON_NUMBER(stream, "rotation", sprite->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "visible", sprite->x, bool)

    chrus_sprite_load(sprite, sprite->source);

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
    chrus_audiostream* sound = chrus_audiostream_create(NULL);

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

    READ_JSON_COLOR(stream, "color", text->color)
    READ_JSON_NUMBER(stream, "x", text->x)
    READ_JSON_NUMBER(stream, "y", text->x)
    READ_JSON_NUMBER(stream, "max_width", text->x)
    READ_JSON_NUMBER(stream, "line_height", text->x)
    READ_JSON_NUMBER_WITH_CAST(stream, "flags", text->flags, int)
    READ_JSON_STRING(stream, "text", text->text)

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
    uint64_t node_id;
    int result;

    /* TODO: figure out what to do if we error */
    current_type = json_next(stream);
    if (current_type != JSON_OBJECT) return NULL;

    is_correct_fieldname(stream, "type");
    current_type = json_next(stream);
    if (current_type != JSON_NUMBER) return NULL;
    node_type = (int)json_get_number(stream);
    

    is_correct_fieldname(stream, "id");
    current_type = json_next(stream);
    if (current_type != JSON_STRING) return NULL;
    /* extracting hexstring into uint64 so that we can use it as a key */
    const char* hexbuffer = json_get_string(stream, NULL);
    sscanf(hexbuffer, "%"SCNx64"", &node_id);

    is_correct_fieldname(stream, "name");
    current_type = json_next(stream);
    if (current_type != JSON_STRING) return NULL;
    node_name = json_get_string_copy(stream);
    /* TODO: string interning? but then garbage collection... */

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

    //current_type = json_peek(stream);

    result = is_correct_fieldname(stream, "children");
    //if (result != 0) return NULL; 
    int children = store_children_in_rbtree((chrus_vector*)&final_node->children, tree, stream);

    final_node->type = node_type; /* set the node type now */
    final_node->name = node_name;

    /* building our treemap now */
    chrus_rbtree_insert_pair(tree, (chrus_rbkey){.keynum=node_id}, final_node);

    current_type = json_next(stream);
    if (current_type != JSON_OBJECT_END) NULL;
    return final_node;
}

static const char* json_get_string_copy(json_stream* stream) {
    const char* buffer;
    const char* real;
    size_t string_size;

    buffer = json_get_string(stream, &string_size);
    real = malloc(sizeof(char)*string_size);
    strcpy(real, buffer);

    return real;
}

static inline int store_children_in_rbtree(chrus_vector* restrict vec, chrus_rbtree* restrict tree, json_stream* restrict stream) {
    /* expects JSON_ARRAY first*/
    enum json_type current_type;
    int children = 0;
    const char* buffer;
    uint64_t child_id;
    chrus_rbnode* data;

    current_type = json_next(stream);
    if (current_type != JSON_ARRAY) return -1;

    while ((current_type = json_next(stream)) == JSON_STRING) {
        buffer = json_get_string(stream, NULL);
        sscanf(buffer, "%"SCNx64"", &child_id);
        data = chrus_rbtree_insert(tree, chrus_rbkey_from_uint(child_id));
        /* incredibly cursed way to store children lol*/
        chrus_vector_append(vec, data);
        children++;
        /* printf("children stored: %d\n", children); */
    }

    if (current_type != JSON_ARRAY_END) return -1;

    return children;
}

static inline int count_children(json_stream* stream) {
    /* expects JSON_ARRAY first*/
    enum json_type current_type;
    int children = 0;

    current_type = json_next(stream);
    if (current_type != JSON_ARRAY) return -1;

    while ((current_type = json_next(stream)) == JSON_STRING) {
        children++;
    }

    if (current_type != JSON_ARRAY_END) return -1;

    return children;
}

chrus_scene* chrus_deserialize_scene(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return NULL;

    //printf("successfully opened scene file\n");

    json_stream first_pass_stream;
    enum json_type current_type;
    chrus_scene* final_scene = NULL;
    chrus_rbtree* pointer_dictionary = chrus_rbtree_create(NULL, NULL, NULL);
    chrus_vector nodes = chrus_vector_create();

    //size_t field_length;
    const char* str_buffer;
    uint64_t scene_id;
    uint64_t camera_id;
    //double number_buffer;
    int result = 0;
    int scene_children;

    json_open_stream(&first_pass_stream, fp);

    // let's make sure we're reading a json array then a json object
    // we need to do some assertions too...
    current_type = json_next(&first_pass_stream);
    if (result || current_type != JSON_ARRAY) goto failure;
    /* reading the scene object now */
    current_type = json_next(&first_pass_stream);
    if (result || current_type != JSON_OBJECT) goto failure;

    result = is_correct_fieldname(&first_pass_stream, "type");
    current_type = json_next(&first_pass_stream);
    if (result || current_type != JSON_NUMBER) goto failure;

    /* can create the scene now */
    result = is_correct_fieldname(&first_pass_stream, "id");
    current_type = json_next(&first_pass_stream);
    if (result || current_type != JSON_STRING) goto failure;
    str_buffer = json_get_string(&first_pass_stream, NULL);
    sscanf(str_buffer, "%"SCNx64"", &scene_id);

    result = is_correct_fieldname(&first_pass_stream, "name");
    current_type = json_next(&first_pass_stream);
    if (current_type != JSON_STRING) goto failure;
    str_buffer = json_get_string_copy(&first_pass_stream);
    final_scene = chrus_scene_create(str_buffer);
    //printf("got past name: %s\n", final_scene->name);

    /* we put ALL pointers in the first pass with the exception of id */
    result = is_correct_fieldname(&first_pass_stream, "current_camera");
    current_type = json_next(&first_pass_stream);
    if (result || current_type != JSON_STRING) goto failure;
    str_buffer = json_get_string(&first_pass_stream, NULL);
    sscanf(str_buffer, "%"SCNx64"", &camera_id);


    result = is_correct_fieldname(&first_pass_stream, "children");
    /* relies on the assumption that sizeof(void**) == sizeof(chrus_node**) */
    scene_children = store_children_in_rbtree((chrus_vector*)&final_scene->children, pointer_dictionary, &first_pass_stream);

    /* 0 <= scene_children <= number of nodes*/
    result = chrus_vector_reserve(&nodes, scene_children); 
    printf("scene children: %d\n", scene_children);

    //printf("red-black tree black height: %d\n", chrus_rbtree_black_height(pointer_dictionary, pointer_dictionary->root));
    //printf("red-black tree valid: %d\n", chrus_rbtree_valid(pointer_dictionary, pointer_dictionary->root));

    current_type = json_next(&first_pass_stream);
    if (current_type != JSON_OBJECT_END) goto failure;

    /* deserialize all nodes in the first pass... */
    chrus_node* current_node;
    int nodes_added = 0;
    while ((current_node = deserialize_object(pointer_dictionary, &first_pass_stream)) != NULL) {
        bool append_success = chrus_vector_append(&nodes, current_node);
        nodes_added += 1;
        if (!append_success) goto failure;
    }

    printf("added %d nodes w/o failure\n", nodes_added);

    /* create the connections, then we can probably pass the scene off here ...? */
    /* second assumption: the deserialized file constitutes a single tree where the root is the scene */
    /* actually this is just correct behavior */
    for (size_t i = 0; i < final_scene->children.size; i++) {
        void** ptr_to_rbnode = (void**)&final_scene->children.data[i];
        chrus_scene_replace_rbnode(final_scene, final_scene, ptr_to_rbnode);
    }
    final_scene->current_camera = chrus_rbtree_find(pointer_dictionary, chrus_rbkey_from_uint(camera_id))->value;

    for (size_t i = 0; i < nodes.size; i++) {
        current_node = nodes.data[i];
        switch (current_node->type)
        {
        default:
            break;
        }
        for (size_t j = 0; j < current_node->children.size; j++) {
            void** ptr_to_rbnode = (void**)&final_scene->children.data[i];
            chrus_scene_replace_rbnode(final_scene, current_node, ptr_to_rbnode);
        }
    }

    goto success;

    failure:
    /* cleanup anything initialized */
    printf("experienced a failure\n");
    json_close(&first_pass_stream);
    fclose(fp);
    chrus_rbtree_destroy(pointer_dictionary);
    if (final_scene) chrus_scene_destroy(final_scene);
    return NULL;
    success:
    json_close(&first_pass_stream);
    fclose(fp);
    chrus_rbtree_destroy(pointer_dictionary);
    return final_scene;
}