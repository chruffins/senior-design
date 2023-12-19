local ffi = require("ffi")

local al_ffi = require("data/allegro_ffi")

local chrus = {}

local function get_line_count(str)
    local lines = 1
    for i = 1, #str do
        local c = str:sub(i, i)
        if c == '\n' then lines = lines + 1 end
    end

    return lines
end

--print(get_line_count(al_ffi.cdef))

chrus.cdef = al_ffi.cdef .. [[
enum CHRUS_NODE_TYPES { CHRUS_NODE_UNINITIALIZED, CHRUS_NODE_CAMERA, CHRUS_NODE_SCRIPT, CHRUS_NODE_SPRITE };

typedef struct chrus_scene_t chrus_scene;
typedef struct chrus_node_t chrus_node;
typedef struct chrus_node_vector_t chrus_node_vec;
typedef struct chrus_camera_t chrus_camera;
typedef struct chrus_script_t chrus_script;
typedef struct chrus_sound_t chrus_sound;
typedef struct chrus_sprite_t chrus_sprite;
typedef struct chrus_vector_t chrus_vector;

struct chrus_vector_t { void **data; size_t size; size_t capacity; };
struct chrus_camera_t { float screen_x, screen_y; float screen_width, screen_height; float viewport_width, viewport_height; float viewport_x, viewport_y; ALLEGRO_TRANSFORM _scaler; };
/*struct chrus_scene_t { const char *name; chrus_node *current_camera; void *lua_vm; chrus_node_vec children; chrus_vector sprites_cache; };*/
struct chrus_node_vector_t { chrus_node **data; size_t size; size_t capacity; };
struct chrus_node_t { chrus_node* parent; chrus_node_vec children; enum CHRUS_NODE_TYPES type; void *data; };
struct chrus_sprite_t { float x; float y; int width; int height; int flipping; float rotation; ALLEGRO_BITMAP *image_data; };

chrus_node* chrus_scene_add_node(chrus_scene* this, void* parent, chrus_node *child);
chrus_sprite* chrus_sprite_create(const char *source);
chrus_node* chrus_node_create_camera();
chrus_node* chrus_node_create_sprite();

chrus_sound *chrus_sound_create(const char *source);

void chrus_sound_play(chrus_sound* restrict this);
void chrus_sound_stop(chrus_sound* restrict this);

]]

return chrus