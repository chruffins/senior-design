local ffi = require("ffi")

local al_ffi = require("allegro_ffi")
ffi.cdef(al_ffi.cdef)
local allegro = ffi.load("allegro")

local width = 800
local height = 600

-- Create a display window
local display = allegro.al_create_display(width, height)
if display == ffi.NULL then
    error("Failed to create display")
end

for i = 1, 255 do
    -- Clear the display to a color (e.g., black)
    allegro.al_clear_to_color(allegro.al_map_rgb(i, i, i)) -- 0 represents black

    -- Update the display
    allegro.al_flip_display()

    -- Wait for a few seconds
    allegro.al_rest(0.01)
end


-- Destroy the display and clean up Allegro
allegro.al_destroy_display(display)