ffi = require("ffi")

al_ffi = require("data/allegro_ffi")

chrus = {}

al = {}
Color = {}
UIDim = {
    abs_x = 0,
    off_x = 0,
    abs_y = 0,
    off_y = 0
}

function UIDim:new(o, ax, ox, ay, oy)
    o = o or {}   -- create object if user does not provide one
    setmetatable(o, self)
    self.__index = self
    o.abs_x = ax
    o.off_x = ox
    o.abs_y = ay
    o.off_y = oy
    return o
end

warn = function(...)
    io.stderr:write("warning: ", ...)
    io.stderr:flush()
end

ffi.cdef(al_ffi.cdef .. [[

void* malloc(size_t size);
void free(void *__ptr);

enum CHRUS_NODE_TYPES { CHRUS_NODE_UNINITIALIZED, CHRUS_NODE_CAMERA, CHRUS_NODE_SCRIPT, CHRUS_NODE_SPRITE, CHRUS_NODE_SOUND, 
CHRUS_NODE_AUDIOSTREAM, CHRUS_NODE_TEXT, CHRUS_NODE_PRIMITIVE, CHRUS_NODE_SHADER, };
enum CHRUS_PRIMITIVE_TYPE {
    CHRUS_PRIMITIVE_HIGHLEVEL,
    CHRUS_PRIMITIVE_LOWLEVEL,
    CHRUS_PRIMITIVE_LOWLEVEL_BACKUP
};

enum CHRUS_PRIM_HL_TYPE {
    CHRUS_PRIM_HL_LINE,
    CHRUS_PRIM_HL_TRIANGLE,
    CHRUS_PRIM_HL_FILLED_TRIANGLE,
    CHRUS_PRIM_HL_RECTANGLE,
    CHRUS_PRIM_HL_FILLED_RECTANGLE,
    CHRUS_PRIM_HL_ROUNDED_RECTANGLE,
    CHRUS_PRIM_HL_FILLED_ROUNDED_RECTANGLE,
    CHRUS_PRIM_HL_PIESLICE,
    CHRUS_PRIM_HL_FILLED_PIESLICE,
    CHRUS_PRIM_HL_ELLIPSE,
    CHRUS_PRIM_HL_FILLED_ELLIPSE,
    CHRUS_PRIM_HL_CIRCLE,
    CHRUS_PRIM_HL_FILLED_CIRCLE,
    CHRUS_PRIM_HL_ARC,
    CHRUS_PRIM_HL_ELLIPTICAL_ARC,
    CHRUS_PRIM_HL_SPLINE,
};

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
typedef struct chrus_primitive_t chrus_prim;
typedef enum CHRUS_PRIMITIVE_TYPE CHRUS_PRIMITIVE_TYPE;
typedef enum CHRUS_PRIM_HL_TYPE CHRUS_PRIM_HL_TYPE;

struct chrus_node_vector_t { chrus_node** data; size_t size; size_t capacity; };
struct chrus_vector_t { void **data; size_t size; size_t capacity; };
struct chrus_camera_t { float screen_x, screen_y; float screen_width, screen_height; float viewport_width, viewport_height; float viewport_x, viewport_y; ALLEGRO_TRANSFORM _scaler; };
struct chrus_scene_t { ALLEGRO_EVENT_SOURCE event_source; const char* _name; chrus_node* current_camera; chrus_node_vec children; chrus_vector drawable_layers[8]; void* lua_vm; ALLEGRO_EVENT_QUEUE* event_queue; ALLEGRO_TIMER* tick_timer; };
struct chrus_node_t { const char *_name; chrus_node* parent; chrus_node_vec children; enum CHRUS_NODE_TYPES type; void *data; };
struct chrus_sprite_t { const char* source; float x; float y; int width; int height; int flipping; float sx; float sy; float rotation; bool visible; ALLEGRO_BITMAP* image_data; };
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

struct chrus_primitive_t {
    CHRUS_PRIMITIVE_TYPE type; /* probably should be erroneous to change this */
    union { struct hl { CHRUS_PRIM_HL_TYPE hl_type; float x1; float y1; float x2; float y2; float x3; float y3; float rx; /* x4 as necessary */
            float ry; float thickness; ALLEGRO_COLOR color;
        } hl;
        struct ll { ALLEGRO_VERTEX_BUFFER* vertex_buffer; ALLEGRO_VERTEX* vertex_backup; ALLEGRO_BITMAP* texture; int start; int end;
            int type; int count; int line_style;
        } ll;
    };
};

chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child);

chrus_node* chrus_node_create_uninit();
chrus_node* chrus_node_create_camera();
chrus_node* chrus_node_create_sprite();

chrus_audiostream *chrus_audiostream_create(const char *source);

void chrus_node_destroy(chrus_node *this);
bool chrus_node_set_name(chrus_node* restrict this, const char* new);

chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_load(chrus_sprite* this, const char *source);
void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);

float chrus_sprite_get_x(chrus_sprite* restrict this);
float chrus_sprite_get_y(chrus_sprite* restrict this);
int chrus_sprite_get_width(chrus_sprite* restrict this);
int chrus_sprite_get_height(chrus_sprite* restrict this);
float chrus_sprite_get_sx(chrus_sprite* restrict this);
float chrus_sprite_get_sy(chrus_sprite* restrict this);
int chrus_sprite_get_flipping(chrus_sprite* restrict this);
float chrus_sprite_get_rotation(chrus_sprite* restrict this);
bool chrus_sprite_get_visible(chrus_sprite* restrict this);
ALLEGRO_BITMAP* chrus_sprite_get_bitmap(chrus_sprite* restrict this);

void chrus_sprite_set_x(chrus_sprite* restrict this, float new);
void chrus_sprite_set_y(chrus_sprite* restrict this, float new);
void chrus_sprite_set_sx(chrus_sprite* restrict this, float new);
void chrus_sprite_set_sy(chrus_sprite* restrict this, float new);
void chrus_sprite_set_flipping(chrus_sprite* restrict this, int new);
void chrus_sprite_set_rotation(chrus_sprite* restrict this, float new);
void chrus_sprite_set_visible(chrus_sprite* restrict this, bool new);
void chrus_sprite_set_bitmap(chrus_sprite* restrict this, ALLEGRO_BITMAP* restrict new);

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
int chrus_text_get_width(chrus_text* restrict this);
int chrus_text_get_height(chrus_text* restrict this);
bool chrus_text_get_visible(chrus_text* restrict this);

void chrus_text_set_color(chrus_text* restrict this, ALLEGRO_COLOR new);
void chrus_text_set_x(chrus_text* restrict this, float new);
void chrus_text_set_y(chrus_text* restrict this, float new);
void chrus_text_set_max_width(chrus_text* restrict this, float new);
void chrus_text_set_line_height(chrus_text* restrict this, float new);
void chrus_text_set_flags(chrus_text* restrict this, int new);
void chrus_text_set_text(chrus_text* restrict this, const char* new);
void chrus_text_set_font(chrus_text* restrict this, const char* font_path, int size);
void chrus_text_set_visible(chrus_text* restrict this, bool new);

void chrus_sound_set_playmode(chrus_sound* restrict this, int new);
void chrus_sound_set_gain(chrus_sound* restrict this, float new);
void chrus_sound_set_pan(chrus_sound* restrict this, float new);
void chrus_sound_set_speed(chrus_sound* restrict this, float new);

void chrus_audiostream_set_playmode(chrus_audiostream* restrict this, int new);
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

chrus_prim* chrus_prim_create();
void chrus_prim_create_vbuffer(chrus_prim* restrict this, int num_vertices, const void* init_data, bool backup);
void chrus_prim_create_hl(chrus_prim* restrict this);

void chrus_prim_draw(chrus_prim* restrict this, float dx, float dy);
bool chrus_prim_translate(chrus_prim* restrict this, float dx, float dy);

bool chrus_prim_get_visible(chrus_prim* restrict this);
bool chrus_prim_get_filled(chrus_prim* restrict this);
bool chrus_prim_set_filled(chrus_prim* restrict this, bool new_value);
bool chrus_prim_set_visible(chrus_prim* restrict this, bool new);
bool chrus_prim_set_hl_type(chrus_prim* restrict this, CHRUS_PRIM_HL_TYPE type);
bool chrus_prim_set_hl_value(chrus_prim* restrict this, int ptr, float value);
bool chrus_prim_set_color(chrus_prim* restrict this, ALLEGRO_COLOR color);

bool chrus_prim_set_line(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_triangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, ALLEGRO_COLOR color);
bool chrus_prim_set_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, ALLEGRO_COLOR color);
bool chrus_prim_set_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, float thickness, ALLEGRO_COLOR color);
bool chrus_prim_set_filled_rounded_rectangle(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float rx, float ry, ALLEGRO_COLOR color);
bool chrus_prim_set_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_pieslice(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color);
bool chrus_prim_set_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_ellipse(chrus_prim* restrict this, float x1, float y1, float rx, float ry, ALLEGRO_COLOR color);
bool chrus_prim_set_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_filled_circle(chrus_prim* restrict this, float x1, float y1, float rx, ALLEGRO_COLOR color);
bool chrus_prim_set_arc(chrus_prim* restrict this, float x1, float y1, float rx, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_elliptical_arc(chrus_prim* restrict this, float x1, float y1, float rx, float ry, float start_theta, float end_theta, ALLEGRO_COLOR color, float thickness);
bool chrus_prim_set_spline(chrus_prim* restrict this, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, ALLEGRO_COLOR color, float thickness);

bool chrus_prim_set_type(chrus_prim* restrict this, int new);

ALLEGRO_VERTEX* chrus_prim_get_vertices(chrus_prim* restrict this);

ALLEGRO_SHADER* chrus_shader_create();
void chrus_scene_set_shader(chrus_scene* restrict this, chrus_node* restrict shader_node, int pos);

void chrus_shader_attach_source_file(ALLEGRO_SHADER* restrict shader, int type, const char* filename);
void chrus_shader_attach_source(ALLEGRO_SHADER* restrict shader, int type, const char* source);
void chrus_shader_build(ALLEGRO_SHADER* shader);

ALLEGRO_BITMAP* chrus_load_bitmap(const char* filename);
void chrus_set_window_title(const char* new_title);
void chrus_set_window_icon(ALLEGRO_BITMAP* icon);

float* chrus_demo_get_audio_buffer();

const char* convert_static_string_to_dynamic(const char*);
]])

--ffi.load("allegro")
local lchrus = ffi.load("chrus_lib")
local lallegro_color = ffi.load("allegro_color")
lallegro = ffi.load("allegro")
--chrus.sound = ffi.typeof("chrus_node")

chrus.set_window_title = function(new_title)
    lchrus.chrus_set_window_title(new_title)
end
chrus.set_window_icon = function(new_icon)
    lchrus.chrus_set_window_icon(new_icon)
end
chrus.get_screen_width = function()
    return 1920
end
chrus.get_screen_height = function()
    return 1080
end

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
            return members[key](node)
        else
            warn(("%s is not a callable function from this node!").format(key))
        end
    end
end

local create_node_newindexfunc = function(newindex_funcs)
    return function (node, key, value)
        return newindex_funcs[key](node, value)
    end
end

Color.rgb = function(r, g, b)
    return lallegro.al_map_rgb(r, g, b)
end

Color.rgba = function(r, g, b, a)
    return lallegro.al_map_rgba(r, g, b, a)
end

Color.hsv = function(h, s, v)
    return lallegro_color.al_color_hsv(h, s, v)
end

load_bitmap = function(path)
    return lchrus.chrus_load_bitmap(path)
end

get_bitmap_flags = function(bmp)
    return lallegro.al_get_bitmap_flags(bmp)
end

get_mixer_buffer = function()
    return lchrus.chrus_demo_get_audio_buffer()
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
    load_file = function(node, source)
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
    x = function(node)
        return lchrus.chrus_sprite_get_x(node.data)
    end,
    y = function(node)
        return lchrus.chrus_sprite_get_y(node.data)
    end,
    width = function(node)
        return lchrus.chrus_sprite_get_width(node.data)
    end,
    height = function(node)
        return lchrus.chrus_sprite_get_height(node.data)
    end,
    flipping = function(node)
        return lchrus.chrus_sprite_get_flipping(node.data)
    end,
    sx = function(node)
        return lchrus.chrus_sprite_get_sx(node.data)
    end,
    sy = function(node)
        return lchrus.chrus_sprite_get_sy(node.data)
    end,
    rotation = function(node)
        return lchrus.chrus_sprite_get_rotation(node.data)
    end,
    visible = function(node)
        return lchrus.chrus_sprite_get_visible(node.data)
    end,
    bitmap = function(node)
        return lchrus.chrus_sprite_get_bitmap(node)
    end,
}

local sprite_newindex_members = {
    bitmap = function(node, data)
        return lchrus.chrus_sprite_set_bitmap(node.data, data)
    end,
    x = function(node, data)
        return lchrus.chrus_sprite_set_x(node.data, data)
    end,
    y = function(node, data)
        return lchrus.chrus_sprite_set_y(node.data, data)
    end,
    width = function(node, data)
        return lchrus.chrus_sprite_set_width(node.data, data)
    end,
    height = function(node, data)
        return lchrus.chrus_sprite_set_height(node.data, data)
    end,
    flipping = function(node, data)
        return lchrus.chrus_sprite_set_flipping(node.data, data)
    end,
    sx = function(node, data)
        return lchrus.chrus_sprite_set_sx(node.data, data)
    end,
    sy = function(node, data)
        return lchrus.chrus_sprite_set_sy(node.data, data)
    end,
    rotation = function(node, data)
        return lchrus.chrus_sprite_set_rotation(node.data, data)
    end,
    visible = function(node, data)
        return lchrus.chrus_sprite_set_visible(node.data, data)
    end,
}

local sprite_index = create_node_indexfunc(sprite_methods, sprite_members)
local sprite_newindex = create_node_newindexfunc(sprite_newindex_members)

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
    font = function(node, font_path, size)
        lchrus.chrus_text_set_font(node.data, font_path, size)
    end,
}

local text_members = {
    color = function(node)
        return lchrus.chrus_text_get_color(node.data)
    end,
    x = function(node)
        return lchrus.chrus_text_get_x(node.data)
    end,
    y = function(node)
        return lchrus.chrus_text_get_y(node.data)
    end,
    max_width = function(node)
        return lchrus.chrus_text_get_max_width(node.data)
    end,
    width = function(node)
        return lchrus.chrus_text_get_width(node.data)
    end,
    height = function(node)
        return lchrus.chrus_text_get_height(node.data)
    end,
    line_height = function(node)
        return lchrus.chrus_text_get_line_height(node.data)
    end,
    text = function(node)
        return lchrus.chrus_text_get_text(node.data)
    end,
    visible = function(node)
        return lchrus.chrus_text_get_visible(node.data)
    end,
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
    position = function(node, value)
        lchrus.chrus_text_set_x(node.data, (value.abs_x * chrus.get_screen_width()) + value.off_x)
        lchrus.chrus_text_set_y(node.data, (value.abs_y * chrus.get_screen_height()) + value.off_y)
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
    justify = function(node, value)
        if value == "left" then
            lchrus.chrus_text_set_flags(node.data, al_ffi.ALLEGRO_ALIGN_LEFT)
        elseif value == "center" then
            lchrus.chrus_text_set_flags(node.data, al_ffi.ALLEGRO_ALIGN_CENTER)
        elseif value == "right" then
            lchrus.chrus_text_set_flags(node.data, al_ffi.ALLEGRO_ALIGN_RIGHT)
        end
    end,
    visible = function(node, value)
        lchrus.chrus_text_set_visible(node.data, value)
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

local primitive_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "prim"
        new.type = lchrus.CHRUS_NODE_PRIMITIVE
        new.parent = nil
        new.data = lchrus.chrus_prim_create()
        return new
    end,
    reparent = function(node, other)
        lchrus.chrus_scene_add_node(scene, other, node)
    end,
    translate = function(node, dx, dy)
        lchrus.chrus_prim_translate(node.data, dx, dy)
    end,
    create_vertex_buffer = function(node, num_vertices)
        -- can't store this stuff in the GPU because lmao never finished it
        lchrus.chrus_prim_create_vbuffer(node.data, num_vertices, nil, true)
    end,
    line = function(node, x1, y1, x2, y2, thickness, color)
        return lchrus.chrus_prim_set_line(node.data, x1, y1, x2, y2, thickness, color)
    end,
    triangle = function(node, x1, y1, x2, y2, x3, y3, thickness, color)
        return lchrus.chrus_prim_set_triangle(node.data, x1, y1, x2, y2, x3, y3, thickness, color)
    end,
    rectangle = function(node, x1, y1, x2, y2, thickness, color)
        return lchrus.chrus_prim_set_rectangle(node.data, x1, y1, x2, y2, thickness, color)
    end,
    rounded_rectangle = function(node, x1, y1, x2, y2, rx, ry, thickness, color)
        return lchrus.chrus_prim_set_rounded_rectangle(node.data, x1, y1, x2, y2, rx, ry, thickness, color)
    end,
    pieslice = function(node, x1, y1, rx, start_angle, delta_angle, thickness, color)
        return lchrus.chrus_prim_set_pieslice(node.data, x1, y1, rx, start_angle, delta_angle, color, thickness)
    end,
    ellipse = function(node, x1, y1, rx, ry, thickness, color)
        return lchrus.chrus_prim_set_ellipse(node.data, x1, y1, rx, ry, color, thickness)
    end,
    circle = function(node, x1, y1, radius, thickness, color)
        return lchrus.chrus_prim_set_circle(node.data, x1, y1, radius, color, thickness)
    end,
    arc = function(node, x1, y1, radius, start_angle, delta_angle, thickness, color)
        return lchrus.chrus_prim_set_arc(node.data, x1, y1, radius, start_angle, delta_angle, color, thickness)
    end,
    elliptical_arc = function(node, x1, y1, rx, ry, start_angle, delta_angle, thickness, color)
        return lchrus.chrus_prim_set_elliptical_arc(node.data, x1, y1, rx, ry, start_angle, delta_angle, color, thickness)
    end,
    spline = function(node, x1, y1, x2, y2, x3, y3, x4, y4, thickness, color)
        return lchrus.chrus_prim_set_spline(node.data, x1, y1, x2, y2, x3, y3, x4, y4, color, thickness)
    end,
}

local primitive_members = {
    filled = function(node)
        return lchrus.chrus_prim_get_filled(node.data)
    end,
    vertices = function(node)
        return lchrus.chrus_prim_get_vertices(node.data)
    end,
    visible = function(node)
        return lchrus.chrus_prim_get_visible(node.data)
    end,
}

local primitive_newindex_members = {
    filled = function(node, data)
        lchrus.chrus_prim_set_filled(node.data, data)
    end,
    x1 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 0, data)
    end,
    y1 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 1, data)
    end,
    x2 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 2, data)
    end,
    y2 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 3, data)
    end,
    x3 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 4, data)
    end,
    y3 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 5, data)
    end,
    x4 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 6, data)
    end,
    y4 = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 7, data)
    end,
    thickness = function(node, data)
        lchrus.chrus_prim_set_hl_value(node.data, 8, data)
    end,
    color = function(node, data)
        lchrus.chrus_prim_set_color(node.data, data)
    end,
    hl_type = function(node, data)
        lchrus.chrus_prim_set_hl_type(node.data, data)
    end,
    ll_type = function(node, data)
        lchrus.chrus_prim_set_type(node.data, data)
    end,
    visible = function(node, data)
        lchrus.chrus_prim_set_visible(node.data, data)
    end,
}

local primitive_index = create_node_indexfunc(primitive_methods, primitive_members)
local primitive_newindex = create_node_newindexfunc(primitive_newindex_members)

local shader_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "shader"
        new.type = lchrus.CHRUS_NODE_SHADER
        new.parent = nil
        new.data = lchrus.chrus_shader_create()
        return new
    end,
    load_file = function(node, shader_type, filename)
        if shader_type == "pixel" then
            lchrus.chrus_shader_attach_source_file(node.data, al_ffi.ALLEGRO_PIXEL_SHADER, filename)
            --lchrus.chrus_shader_build(node.data)
        elseif shader_type == "vertex" then
            lchrus.chrus_shader_attach_source_file(node.data, al_ffi.ALLEGRO_VERTEX_SHADER, filename)
            --lchrus.chrus_shader_build(node.data)
        end
    end,
    load = function(node, shader_type, source_code)
        if shader_type == "pixel" then
            lchrus.chrus_shader_attach_source(node.data, al_ffi.ALLEGRO_PIXEL_SHADER, source_code)
            --lchrus.chrus_shader_build(node.data)
        elseif shader_type == "vertex" then
            lchrus.chrus_shader_attach_source(node.data, al_ffi.ALLEGRO_VERTEX_SHADER, source_code)
            --lchrus.chrus_shader_build(node.data)
        end
    end,
    build = function(node)
        lchrus.chrus_shader_build(node.data)
    end,
    use = function(node)
        -- TODO: tell the scene to be using this shader from now on
        --lchrus.chrus_shader_build(node.data)
        lchrus.chrus_scene_set_shader(scene, node, 0)
    end,
    stop_using = function(node)
        lchrus.chrus_scene_set_shader(scene, nil, 0)
    end,
    set_float = function(node, fieldname, value)
        lallegro.al_set_shader_float(fieldname, value)
    end,
}

local shader_members = {

}

local shader_newindex_members = {

}

local shader_index = create_node_indexfunc(shader_methods, shader_members)
local shader_newindex = create_node_newindexfunc(shader_newindex_members)

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
local prim_enum = tonumber(lchrus.CHRUS_NODE_PRIMITIVE)
local shader_enum = tonumber(lchrus.CHRUS_NODE_SHADER)

local index_table = {
    [sprite_enum] = sprite_index,
    [audiostream_enum] = audiostream_index,
    [script_enum] = script_index,
    [text_enum] = text_index,
    [prim_enum] = primitive_index,
    [shader_enum] = shader_index,
}

local newindex_table = {
    [text_enum] = text_newindex,
    [audiostream_enum] = audiostream_newindex,
    [sprite_enum] = sprite_newindex,
    [prim_enum] = primitive_newindex,
    [shader_enum] = shader_newindex,
}

local node_index = {
    name = function(node)
        return ffi.string(node._name)
    end,
}

local node_newindex = {
    name = function(node, value)
        lchrus.chrus_node_set_name(node, value)
    end,
}

local node_metatable = {
    __index = function (node, key)
        if node_index[key] then
            return node_index[key](node)
        end

        return index_table[tonumber(node.type)](node, key)
    end,
    __newindex = function (node, key, value)
        if node_newindex[key] then
            return node_newindex[key](node, value)
        end

        return newindex_table[tonumber(node.type)](node, key, value)
    end
}

local type_table = {
    audiostream = audiostream_methods.new,
    sprite = sprite_methods.new,
    text = text_methods.new,
    primitive = primitive_methods.new,
    shader = shader_methods.new,
}

local scene_index_methods = {
    get_child = function(scene, name)
        for i = 0, tonumber(scene.children.size) - 1 do
            if scene.children.data[i].name == name then
                return scene.children.data[i]
            end
        end
        return nil
    end,
}

local scene_index_members = {
    name = function(scene)
        return ffi.string(scene._name)
    end
}

local scene_newindex_members = {
    name = function(scene, new_name)
        -- do stuff lmao
    end
}

local scene_index = create_node_indexfunc(scene_index_methods, scene_index_members)
local scene_newindex = create_node_newindexfunc(scene_newindex_members)

local scene_metatable = {
    __index = scene_index,
    __newindex = scene_newindex,
}

--local mouse = {clicked = event:new(), leftclicked = event:new(), rightclicked = event:new() }

function create_node(type)
    return type_table[type]()
end

chrus.node = ffi.metatype("chrus_node", node_metatable)
chrus.scene = ffi.metatype("chrus_scene", scene_metatable)

scene = ffi.cast("chrus_scene*", scene)
print("Lua VM fully loaded.")