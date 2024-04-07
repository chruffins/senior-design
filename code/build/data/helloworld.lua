local sansfont_path = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
local font_path = "/home/chris/.local/share/fonts/David-Regular.otf"
local sound = create_node("audiostream")
local puff_path = "data/puff.jpg"

local puff_bitmap = load_bitmap(puff_path)
if puff_bitmap == nil then
    print("puff failed to load")
else
    print("puff is READY")
end

local stupid_icon = load_bitmap("data/fudge.webp")

sound:load("data/vine.ogg")
sound:play()
sound:reparent(scene)

local background_rect = create_node("primitive")
background_rect:rectangle(-1, -1, 1920, 1080, 1, Color.rgb(100, 100, 180))
background_rect.filled = true
background_rect:reparent(scene)

local welcome_text = create_node("text")
welcome_text:font(font_path, 72)
welcome_text.text = "Welcome to Chrus Engine!"
welcome_text.x = 1920 / 2
welcome_text.max_width = 600
welcome_text.justify = "center"
welcome_text:reparent(scene)

--local puff_sprite = create_node("sprite")
--puff_sprite.bitmap = puff_bitmap
--puff_sprite:reparent(scene)
--puff_sprite.x = (1920 - puff_sprite.width) / 2
--puff_sprite.y = (1080 - puff_sprite.height) / 2

local test_shader = create_node("shader")
test_shader:load_file("pixel", "data/pink_shader.fs")
test_shader:load_file("vertex", "data/vertex.vs")
test_shader:build()

local welcome_menu_frame = create_node("primitive")
welcome_menu_frame:rounded_rectangle((1920 / 2) - 250, (1080 / 2) - 250, (1920 / 2) + 250, (1080 / 2) + 200, 10, 10, 1, Color.rgba(0, 0, 0, 100))
welcome_menu_frame.filled = true
welcome_menu_frame:reparent(scene)

local welcome_menu_about = create_node("text")
welcome_menu_about.position = UIDim:new(nil, 0.5, 0, 0.5, 0)
welcome_menu_about:font(font_path, 40)
welcome_menu_about.text = "About"
welcome_menu_about.justify = "center"
welcome_menu_about:reparent(scene)

local about_menu_text = create_node("text")
about_menu_text.position = UIDim:new(nil, 0.5, 0, 0.3, 0)
about_menu_text:font(sansfont_path, 16)
about_menu_text.justify = "center"
about_menu_text.max_width = 450
about_menu_text.text = "Welcome to the Chrus Engine demo! Chrus Engine is a 2D game engine. It's intended to be a lightweight, cross-platform engine suitable for general purposes!"
about_menu_text.visible = false
about_menu_text:reparent(scene)

chrus.set_window_title("Chrus Engine Demo")
chrus.set_window_icon(stupid_icon)

get_mouse().moved:connect(function(x, y)
    if not welcome_menu_about.visible then return end

    if welcome_menu_about.x - welcome_menu_about.width <= x and x <= welcome_menu_about.x + welcome_menu_about.width 
        and welcome_menu_about.y - welcome_menu_about.height <= y and y <= welcome_menu_about.y + welcome_menu_about.height * 2 then
        welcome_menu_about.color = Color.rgb(255, 255, 150)
    else
        welcome_menu_about.color = Color.rgb(255, 255, 255)
    end
end)

get_mouse().clicked:connect(function(x, y)
    if not welcome_menu_about.visible then return end

    if welcome_menu_about.x - welcome_menu_about.width <= x and x <= welcome_menu_about.x + welcome_menu_about.width 
        and welcome_menu_about.y - welcome_menu_about.height <= y and y <= welcome_menu_about.y + welcome_menu_about.height * 2 then
        about_menu_text.visible = not about_menu_text.visible
    end
end)

local shader_enabled = false
get_keyboard().keydown:connect(function(keycode)
    if keycode == al_ffi.ALLEGRO_KEY_Q then
        print(shader_enabled)
        shader_enabled = not shader_enabled
        if shader_enabled then
            test_shader:use()
        else
            test_shader:stop_using()
        end
    end
end)
--test_shader:use()

--[[
print("Hello world!")

local sound = create_node("audiostream")

sound:load("data/vine.ogg")
sound:play()
sound:reparent(scene)

local pink_shader_code = [[
#ifdef GL_ES
precision lowp float;
#endif
uniform sampler2D al_tex;
uniform bool al_use_tex;
uniform bool al_alpha_test;
uniform int al_alpha_func;
uniform float al_alpha_test_val;
varying vec4 varying_color;
varying vec2 varying_texcoord;

bool alpha_test_func(float x, int op, float compare);

void main()
{
  vec4 c;
  if (al_use_tex)
    c = varying_color * texture2D(al_tex, varying_texcoord);
  else
    c = varying_color;
  if (!al_alpha_test || alpha_test_func(c.a, al_alpha_func, al_alpha_test_val))
    gl_FragColor = c; //vec4(1.0,0.0,1.0,1.0);
  else
    discard;
}

bool alpha_test_func(float x, int op, float compare)
{
  if (op == 0) return false;
  else if (op == 1) return true;
  else if (op == 2) return x < compare;
  else if (op == 3) return x == compare;
  else if (op == 4) return x <= compare;
  else if (op == 5) return x > compare;
  else if (op == 6) return x != compare;
  else if (op == 7) return x >= compare;
  return false;
}


local texts = {}
local sprites = {}
local prims = {}
local cap = 20
local keyboard = get_keyboard()

local prim_cap = 8

local strings = {
    "Hello world!",
    "I love life!",
    "Die."
}

local font_path = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"


local test_shader = create_node("shader")
test_shader:load_file("pixel", "data/pink_shader.fs")
test_shader:load_file("vertex", "data/vertex.vs")
test_shader:build()

get_tick().tick:connect(function(current_time)
    test_shader:set_float("u_time", current_time)
end)

keyboard.keydown:connect(function(keycode)
    if keycode == al_ffi.ALLEGRO_KEY_S then
        sound:stop()
    elseif keycode == al_ffi.ALLEGRO_KEY_P then
        sound:play()
    end
end)

get_mouse().clicked:connect(function(x, y)
    local text = create_node("text")
    text.text = string.format("(%d, %d)", x, y)
    text.x = x
    text.y = y
    text.color = Color.rgb(255, 255, 255)
    text:font(font_path, 24)
    text:reparent(scene)
end)

local clicks = 0
local triangle_maker

get_mouse().clicked:connect(function(x, y)
    if clicks == 0 then
        triangle_maker = create_node("primitive")
        triangle_maker.thickness = 3
        triangle_maker.x1 = x
        triangle_maker.y1 = y
    elseif clicks == 1 then
        triangle_maker.x2 = x
        triangle_maker.y2 = y
    elseif clicks == 2 then
        triangle_maker.x3 = x
        triangle_maker.y3 = y
        triangle_maker.color = Color.rgb(150 + math.random(100), 150 + math.random(100), 150 + math.random(100))
        triangle_maker.hl_type = 1
        triangle_maker:reparent(scene)
    end
    clicks = (clicks + 1) % 3
end)
]]