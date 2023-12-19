local ffi = require("ffi")
local os = require("os")

--os.execute("ls")

local chrus_ffi = require("data/chrus_ffi")
chrus_lib = ffi.load("chrus_lib")
ffi.cdef(chrus_ffi.cdef)

--scene = ffi.cast("chrus_scene*", scene)
--sprite = chrus_lib.chrus_node_create_sprite()

--sound = chrus_lib.
--chrus_lib.chrus_sound_play(sound)
--chrus_lib.chrus_scene_add_node(scene, scene, sprite)