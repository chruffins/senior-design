local ffi = require("ffi")

local al_ffi = require("data/allegro_ffi")

chrus = {}
al = {}

ffi.cdef(al_ffi.cdef .. [[

void* malloc(size_t size);
void free(void *__ptr);

enum CHRUS_NODE_TYPES { CHRUS_NODE_UNINITIALIZED, CHRUS_NODE_CAMERA, CHRUS_NODE_SCRIPT, CHRUS_NODE_SPRITE, CHRUS_NODE_SOUND };

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

struct chrus_vector_t { void **data; size_t size; size_t capacity; };
struct chrus_camera_t { float screen_x, screen_y; float screen_width, screen_height; float viewport_width, viewport_height; float viewport_x, viewport_y; ALLEGRO_TRANSFORM _scaler; };
/*struct chrus_scene_t { const char *name; chrus_node *current_camera; void *lua_vm; chrus_node_vec children; chrus_vector sprites_cache; };*/
struct chrus_node_vector_t { chrus_node **data; size_t size; size_t capacity; };
struct chrus_node_t { const char *name; chrus_node* parent; chrus_node_vec children; enum CHRUS_NODE_TYPES type; void *data; };
struct chrus_sprite_t { float x; float y; int width; int height; int flipping; float rotation; ALLEGRO_BITMAP *image_data; };

chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child);

chrus_node* chrus_node_create_camera();
chrus_node* chrus_node_create_sprite();

chrus_audiostream *chrus_audiostream_create(const char *source);

void chrus_node_destroy(chrus_node *this);

chrus_sprite* chrus_sprite_create(const char *source);
void chrus_sprite_translate(chrus_sprite *this, float dx, float dy);

void chrus_audiostream_load(chrus_audiostream* restrict this, const char *source);
void chrus_audiostream_play(chrus_audiostream* restrict this);
void chrus_audiostream_stop(chrus_audiostream* restrict this);
void chrus_audiostream_free(chrus_audiostream* restrict this);

]])

--ffi.load("allegro")
local lchrus = ffi.load("chrus_lib")
lallegro = ffi.load("allegro")
--chrus.sound = ffi.typeof("chrus_node")

local function custom_alloc(typestr, finalizer)
    -- use free as the default finalizer
    if not finalizer then finalizer = ffi.C.free end

    -- automatically construct the pointer type from the base type
    local ptr_typestr = ffi.typeof(typestr .. "*")

    -- how many bytes to allocate?
    local typesize    = ffi.sizeof(typestr)

    -- do the allocation and cast the pointer result
    local ptr = ffi.cast(ptr_typestr, ffi.C.malloc(typesize))

    -- install the finalizer
    ffi.gc( ptr, finalizer )
    return ptr
end


local finalizer = function (this)
    if this.parent == nil then
        print(tostring(this.type) .. " node garbage collected")
        lchrus.chrus_node_destroy(this)
    else
        --print("lua gc is trying to clean this object up but it's part of the scene")
    end
end

--local node = custom_alloc("chrus_node", finalizer)

local sprite_metatable = {
    new = function()
        local new = custom_alloc("chrus_node", finalizer)
        new.name = "sprite"
        new.type = lchrus.CHRUS_NODE_SPRITE
        new.parent = nil
        new.data = lchrus.chrus_sprite_create(nil)
        return new
    end,
    move = function(this, x, y)
        lchrus.chrus_sprite_translate(this.data, x, y)
    end,
    reparent = function(this, other)
        lchrus.chrus_scene_add_node(scene, other, this)
    end,
}

local sound_metatable = {
    new = function()
        local new = custom_alloc("chrus_node", finalizer)
        new.name = "sound"
        new.type = lchrus.CHRUS_NODE_SOUND
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
    volume = function(this)
        return this.data.volume
    end
}

local sound_enum = tonumber(lchrus.CHRUS_NODE_SOUND)
local sprite_enum = tonumber(lchrus.CHRUS_NODE_SPRITE)

local lookup_table = {
    [sound_enum] = sound_metatable,
    [sprite_enum] = sprite_metatable
}

local test_metatable = {
    __index = function (table, key)
        return lookup_table[tonumber(table.type)][key]
    end
}

local type_table = {
    sound = sound_metatable.new,
    sprite = sprite_metatable.new
}

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