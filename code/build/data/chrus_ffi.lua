ffi = require("ffi")

al_ffi = require("data/allegro_ffi")

chrus = {}
al = {}

ffi.cdef(al_ffi.cdef .. [[

void* malloc(size_t size);
void free(void *__ptr);

enum CHRUS_NODE_TYPES { CHRUS_NODE_UNINITIALIZED, CHRUS_NODE_CAMERA, CHRUS_NODE_SCRIPT, CHRUS_NODE_SPRITE, CHRUS_NODE_SOUND, CHRUS_NODE_AUDIOSTREAM, CHRUS_NODE_TEXT, };

typedef struct chrus_scene_t chrus_scene;
typedef struct chrus_node_t chrus_node;
typedef struct chrus_node_vector_t chrus_node_vec;
typedef struct chrus_camera_t chrus_camera;
typedef struct chrus_script_t chrus_script;
typedef struct chrus_audiostream_t chrus_audiostream;
typedef struct chrus_sprite_t chrus_sprite;
typedef struct chrus_vector_t chrus_vector;

typedef struct chrus_node_t chrus_audiostream_node;
typedef struct chrus_node_t chrus_sprite_node;
typedef struct chrus_text_t chrus_text;
typedef struct chrus_sound_t chrus_sound;

struct chrus_node_vector_t { chrus_node** data; size_t size; size_t capacity; };
struct chrus_vector_t { void **data; size_t size; size_t capacity; };
struct chrus_camera_t { float screen_x, screen_y; float screen_width, screen_height; float viewport_width, viewport_height; float viewport_x, viewport_y; ALLEGRO_TRANSFORM _scaler; };
struct chrus_scene_t { ALLEGRO_EVENT_SOURCE event_source; const char* name; chrus_node* current_camera; chrus_node_vec children; chrus_vector sprites_cache; void* lua_vm; ALLEGRO_EVENT_QUEUE* event_queue; ALLEGRO_TIMER* tick_timer; };
struct chrus_node_t { const char *name; chrus_node* parent; chrus_node_vec children; enum CHRUS_NODE_TYPES type; void *data; };
struct chrus_sprite_t { float x; float y; int width; int height; int flipping; float rotation; ALLEGRO_BITMAP *image_data; };
struct chrus_text_t {
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR color;
    float x;
    float y;
    float max_width;
    float line_height;
    int flags;
    const char* text;
};
struct chrus_sound_t {
    ALLEGRO_SAMPLE* sample;
    ALLEGRO_SAMPLE_INSTANCE* sound;
    float length;
    int playmode;
    float gain;
    float pan;
    float speed;
};

chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child);

chrus_node* chrus_node_create_uninit();
chrus_node* chrus_node_create_camera();
chrus_node* chrus_node_create_sprite();

chrus_audiostream *chrus_audiostream_create(const char *source);

void chrus_node_destroy(chrus_node *this);

chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_load(chrus_sprite* this, const char *source);
void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);

const char* chrus_script_get_source(chrus_script* restrict this);

void chrus_sound_load(chrus_sound* restrict this, const char *source);
void chrus_sound_play(chrus_sound* restrict this);
void chrus_sound_stop(chrus_sound* restrict this);
void chrus_sound_destroy(chrus_sound* restrict this);

void chrus_audiostream_load(chrus_audiostream* restrict this, const char *source);
void chrus_audiostream_play(chrus_audiostream* restrict this);
void chrus_audiostream_stop(chrus_audiostream* restrict this);
void chrus_audiostream_free(chrus_audiostream* restrict this);

chrus_text* chrus_text_create();
void chrus_text_draw(chrus_text* restrict this);
void chrus_text_destroy(chrus_text* restrict this);

ALLEGRO_COLOR chrus_text_get_color(chrus_text* restrict this);
float chrus_text_get_x(chrus_text* restrict this);
float chrus_text_get_y(chrus_text* restrict this);
float chrus_text_get_max_width(chrus_text* restrict this);
float chrus_text_get_line_height(chrus_text* restrict this);
int chrus_text_get_flags(chrus_text* restrict this);
const char* chrus_text_get_text(chrus_text* restrict this);

void chrus_text_set_color(chrus_text* restrict this, ALLEGRO_COLOR new);
void chrus_text_set_x(chrus_text* restrict this, float new);
void chrus_text_set_y(chrus_text* restrict this, float new);
void chrus_text_set_max_width(chrus_text* restrict this, float new);
void chrus_text_set_line_height(chrus_text* restrict this, float new);
void chrus_text_set_flags(chrus_text* restrict this, int new);
void chrus_text_set_text(chrus_text* restrict this, const char* new);

void chrus_sound_set_playmode(chrus_sound* restrict this, ALLEGRO_PLAYMODE new);
void chrus_sound_set_gain(chrus_sound* restrict this, float new);
void chrus_sound_set_pan(chrus_sound* restrict this, float new);
void chrus_sound_set_speed(chrus_sound* restrict this, float new);

void chrus_audiostream_set_playmode(chrus_audiostream* restrict this, ALLEGRO_PLAYMODE new);
void chrus_audiostream_set_gain(chrus_audiostream* restrict this, float new);
void chrus_audiostream_set_pan(chrus_audiostream* restrict this, float new);
void chrus_audiostream_set_speed(chrus_audiostream* restrict this, float new);

float chrus_sound_get_length(chrus_sound* restrict this);
ALLEGRO_PLAYMODE chrus_sound_get_playmode(chrus_sound* restrict this);
float chrus_sound_get_gain(chrus_sound* restrict this);
float chrus_sound_get_pan(chrus_sound* restrict this);
float chrus_sound_get_speed(chrus_sound* restrict this);

float chrus_audiostream_get_length(chrus_audiostream* restrict this);
ALLEGRO_PLAYMODE chrus_audiostream_get_playmode(chrus_audiostream* restrict this);
float chrus_audiostream_get_gain(chrus_audiostream* restrict this);
float chrus_audiostream_get_pan(chrus_audiostream* restrict this);
float chrus_audiostream_get_speed(chrus_audiostream* restrict this);

]])

--ffi.load("allegro")
local lchrus = ffi.load("chrus_lib")
lallegro = ffi.load("allegro")
--chrus.sound = ffi.typeof("chrus_node")

local function custom_node_alloc(typestr, finalizer)
    -- use free as the default finalizer
    if not finalizer then finalizer = ffi.C.free end

    -- automatically construct the pointer type from the base type
    local ptr_typestr = ffi.typeof(typestr .. "*")

    -- how many bytes to allocate?
    --local typesize    = ffi.sizeof(typestr)

    -- do the allocation and cast the pointer result
    local ptr = ffi.cast(ptr_typestr, lchrus.chrus_node_create_uninit())

    -- install the finalizer
    ffi.gc( ptr, finalizer )
    return ptr
end

local finalizer = function (node)
    if node.parent == nil then
        print(tostring(node.type) .. " node garbage collected")
        lchrus.chrus_node_destroy(node)
    else
    end
end

local create_node_indexfunc = function(methods, members)
    return function (node, key)
        if methods[key] then
            return methods[key]
        elseif members[key] then
            return members[key](node.data)
        end
    end
end

local create_node_newindexfunc = function(newindex_funcs)
    return function (node, key, value)
        return newindex_funcs[key](node, value)
    end
end

local sprite_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "sprite"
        new.type = lchrus.CHRUS_NODE_SPRITE
        new.parent = nil
        new.data = lchrus.chrus_sprite_create(nil)
        return new
    end,
    load = function(node, source)
        lchrus.chrus_sprite_load(node.data, source)
    end,
    move = function(node, x, y)
        lchrus.chrus_sprite_translate(node.data, x, y)
    end,
    reparent = function(node, other)
        lchrus.chrus_scene_add_node(scene, other, node)
    end,
}

local sprite_members = {

}

local sprite_index = create_node_indexfunc(sprite_methods, sprite_members)

local text_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "text"
        new.type = lchrus.CHRUS_NODE_TEXT
        new.parent = nil
        new.data = lchrus.chrus_text_create()
        return new
    end,
    reparent = function(node, other)
        lchrus.chrus_scene_add_node(scene, other, node)
    end,
}

local text_members = {
    text = function(node)
        return lchrus.chrus_text_get_text(node)
    end
}

local text_index = create_node_indexfunc(text_methods, text_members)

local text_newindex_members = {
    color = function(node, value)
        lchrus.chrus_text_set_color(node.data, value)
    end,
    x = function(node, value)
        lchrus.chrus_text_set_x(node.data, value)
    end,
    y = function(node, value)
        lchrus.chrus_text_set_y(node.data, value)
    end,
    max_width = function(node, value)
        lchrus.chrus_text_set_max_width(node.data, value)
    end,
    line_height = function(node, value)
        lchrus.chrus_text_set_line_height(node.data, value)
    end,
    text = function(node, value)
        lchrus.chrus_text_set_text(node.data, value)
    end,

}

local text_newindex = create_node_newindexfunc(text_newindex_members)

local audiostream_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "sound"
        new.type = lchrus.CHRUS_NODE_AUDIOSTREAM
        new.parent = nil
        new.data = lchrus.chrus_audiostream_create(nil)
        return new
    end,
    load = function(node, source)
        lchrus.chrus_audiostream_load(node.data, source)
    end,
    play = function(node)
        lchrus.chrus_audiostream_play(node.data)
    end,
    stop = function(node)
        lchrus.chrus_audiostream_stop(node.data)
    end,
    destroy = function(node)
        lchrus.chrus_audiostream_free(node.data)
        node = nil
    end,
    reparent = function(node, other)
        lchrus.chrus_scene_add_node(scene, other, node)
    end,
}

local audiostream_members = {
    volume = function(node)
        return lchrus.chrus_audiostream_get_volume(node.data)
    end,
    playmode = function(node)
        return lchrus.chrus_audiostream_get_playmode(node.data)
    end,
    gain = function(node)
        return lchrus.chrus_audiostream_get_gain(node.data)
    end,
    pan = function(node)
        return lchrus.chrus_audiostream_get_pan(node.data)
    end,
    speed = function(node)
        return lchrus.chrus_audiostream_get_speed(node.data)
    end,
}

local audiostream_newindex_members = {
    volume = function(node, value)
        lchrus.chrus_audiostream_set_volume(node.data, value)
    end,
    playmode = function(node, value)
        lchrus.chrus_audiostream_set_playmode(node.data, value)
    end,
    gain = function(node, value)
        lchrus.chrus_audiostream_set_gain(node.data, value)
    end,
    pan = function(node, value)
        lchrus.chrus_audiostream_set_pan(node.data, value)
    end,
    speed = function(node, value)
        lchrus.chrus_audiostream_set_speed(node.data, value)
    end,
}

local audiostream_index = create_node_indexfunc(audiostream_methods, audiostream_members)
local audiostream_newindex = create_node_newindexfunc(audiostream_newindex_members)

local script_methods = {
    get_source = function(node)
        return lchrus.chrus_script_get_source(node.data)
    end
}

local script_index = function(table, key)
    if script_methods[key] then
        return script_methods[key]
    end
end

local audiostream_enum = tonumber(lchrus.CHRUS_NODE_AUDIOSTREAM)
local sprite_enum = tonumber(lchrus.CHRUS_NODE_SPRITE)
local script_enum = tonumber(lchrus.CHRUS_NODE_SCRIPT)
local text_enum = tonumber(lchrus.CHRUS_NODE_TEXT)

local index_table = {
    [sprite_enum] = sprite_index,
    [audiostream_enum] = audiostream_index,
    [script_enum] = script_index,
    [text_enum] = text_index,
}

local newindex_table = {
    [text_enum] = text_newindex,
    [audiostream_enum] = audiostream_newindex,
}

local test_metatable = {
    __index = function (node, key)
        return index_table[tonumber(node.type)](node, key)
    end,
    __newindex = function (node, key, value)
        return newindex_table[tonumber(node.type)](node, key, value)
    end
}

local type_table = {
    audiostream = audiostream_methods.new,
    sprite = sprite_methods.new,
    text = text_methods.new,
}

--local mouse = {clicked = event:new(), leftclicked = event:new(), rightclicked = event:new() }

function create_node(type)
    return type_table[type]()
end

--chrus.sound = {}
--chrus.sprite = {}
--setmetatable(chrus.sound, sound_metatable)
--setmetatable(chrus.sprite, sprite_metatable)
chrus.node = ffi.metatype("chrus_node", test_metatable)
--chrus.sound = ffi.metatype("chrus_node", test_metatable)
--chrus.sprite = ffi.metatype("chrus_node", sprite_metatable)
print("Lua VM fully loaded.")