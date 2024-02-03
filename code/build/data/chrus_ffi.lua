ffi = require("ffi")

local al_ffi = require("data/allegro_ffi")

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

void chrus_audiostream_load(chrus_audiostream* restrict this, const char *source);
void chrus_audiostream_play(chrus_audiostream* restrict this);
void chrus_audiostream_stop(chrus_audiostream* restrict this);
void chrus_audiostream_free(chrus_audiostream* restrict this);

chrus_text* chrus_text_create();
void chrus_text_draw(chrus_text* restrict this);
void chrus_text_destroy(chrus_text* restrict this);
const char* chrus_text_get_text(chrus_text* restrict this);

void chrus_text_set_text(chrus_text* restrict this, const char* new_text);

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

local finalizer = function (this)
    if this.parent == nil then
        print(tostring(this.type) .. " node garbage collected")
        lchrus.chrus_node_destroy(this)
    else
    end
end

local sprite_metatable = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "sprite"
        new.type = lchrus.CHRUS_NODE_SPRITE
        new.parent = nil
        new.data = lchrus.chrus_sprite_create(nil)
        return new
    end,
    load = function(this, source)
        lchrus.chrus_sprite_load(this.data, source)
    end,
    move = function(this, x, y)
        lchrus.chrus_sprite_translate(this.data, x, y)
    end,
    reparent = function(this, other)
        lchrus.chrus_scene_add_node(scene, other, this)
    end,
}

-- members need to have .member but implicitly call a function
-- methods can be written the same way
-- text_functions(table, key)
local text_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "text"
        new.type = lchrus.CHRUS_NODE_TEXT
        new.parent = nil
        new.data = lchrus.chrus_text_create()
        return new
    end,
    reparent = function(this, other)
        lchrus.chrus_scene_add_node(scene, other, this)
    end,
}

local text_members = {
    text = function(this)
        return lchrus.chrus_text_get_text(this)
    end
}

local create_node_indexfunc = function(methods, members)
    return function (node, key)
        if methods[key] then
            return methods[key]
        elseif members[key] then
            return members[key](node.data)
        end
    end
end

local text_index = create_node_indexfunc(text_methods, text_members)

local text_newindex = function(node, key, value)
    if key == "text" then
        lchrus.chrus_text_set_text(node.data, value)
    end
end

local audiostream_methods = {
    new = function()
        local new = custom_node_alloc("chrus_node", finalizer)
        new.name = "sound"
        new.type = lchrus.CHRUS_NODE_AUDIOSTREAM
        new.parent = nil
        new.data = lchrus.chrus_audiostream_create(nil)
        return new
    end,
    load = function(this, source)
        lchrus.chrus_audiostream_load(this.data, source)
    end,
    play = function(this)
        lchrus.chrus_audiostream_play(this.data)
    end,
    stop = function(this)
        lchrus.chrus_audiostream_stop(this.data)
    end,
    destroy = function(this)
        lchrus.chrus_audiostream_free(this.data)
        this = nil
    end,
    reparent = function(this, other)
        lchrus.chrus_scene_add_node(scene, other, this)
    end,
}

local audiostream_members = {
    volume = function(this)
        return this.volume
    end
}

local audiostream_index = create_node_indexfunc(audiostream_methods, audiostream_members)

local script_methods = {
    get_source = function(this)
        return lchrus.chrus_script_get_source(this.data)
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
    --[[
    
    [sprite_enum] = sprite_metatable,
    ]]
    [audiostream_enum] = audiostream_index,
    [script_enum] = script_index,
    [text_enum] = text_index,
}

local newindex_table = {
    [text_enum] = text_newindex,
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
    sprite = sprite_metatable.new,
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