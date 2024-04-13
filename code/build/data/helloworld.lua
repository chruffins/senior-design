local sansfont_path = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
local font_path = "/home/chris/.local/share/fonts/David-Regular.otf"
local sound = create_node("audiostream")
local cat_path = "data/test.png"
local puff_path = "data/puff.jpg"
local green = Color.rgb(0, 255, 0)

local cat_bitmap = load_bitmap(cat_path)
local puff_bitmap = load_bitmap(puff_path)
if puff_bitmap == nil then
    print("puff failed to load")
else
    print("puff is READY")
    print(puff_bitmap)
end

local stupid_icon = load_bitmap("data/fudge.webp")

sound:load("data/hummingbird.ogg")
sound.playmode = al_ffi.ALLEGRO_PLAYMODE_LOOP
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

local test_shader = create_node("shader")
test_shader:load_file("pixel", "data/pink_shader.fs")
test_shader:load_file("vertex", "data/vertex.vs")
test_shader:build()

local welcome_menu_frame = create_node("primitive")
welcome_menu_frame:rounded_rectangle((1920 / 2) - 250, (1080 / 2) - 250, (1920 / 2) + 250, (1080 / 2) + 200, 10, 10, 1, Color.rgba(0, 0, 0, 100))
welcome_menu_frame.filled = true
welcome_menu_frame:reparent(scene)

local welcome_menu_demos = create_node("text")
welcome_menu_demos.position = UIDim:new(nil, 0.5, 0, 0.3, 0)
welcome_menu_demos:font(font_path, 64)
welcome_menu_demos.text = "Demos"
welcome_menu_demos.justify = "center"
welcome_menu_demos:reparent(scene)

local welcome_menu_about = create_node("text")
welcome_menu_about.position = UIDim:new(nil, 0.5, 0, 0.6, 0)
welcome_menu_about:font(font_path, 64)
welcome_menu_about.text = "About"
welcome_menu_about.justify = "center"
welcome_menu_about:reparent(scene)

local about_menu_back = create_node("text")
about_menu_back.position = UIDim:new(nil, 0.5, 0, 0.6, 0)
about_menu_back:font(font_path, 64)
about_menu_back.text = "Back"
about_menu_back.justify = "center"
about_menu_back.visible = false
about_menu_back:reparent(scene)

local about_menu_puff = create_node("sprite")
about_menu_puff.bitmap = cat_bitmap
about_menu_puff:reparent(scene)
about_menu_puff.x = (1920 - about_menu_puff.width / 8) / 2
about_menu_puff.y = (1080 - about_menu_puff.height / 8) / 2
about_menu_puff.sx = 1
about_menu_puff.sy = 1
about_menu_puff.rotation = 0
about_menu_puff.visible = false

local about_menu_text = create_node("text")
about_menu_text.position = UIDim:new(nil, 0.5, 0, 0.3, 0)
about_menu_text:font(sansfont_path, 16)
about_menu_text.justify = "center"
about_menu_text.max_width = 450
about_menu_text.text = [[Welcome to the Chrus Engine demo! This demo is intended to showcase Chrus Engine's various features-- you've already seen sprites, text, shapes and scripting!

By Chris Lee
Project Advisor: Dr. Badri Vellambi
]]
about_menu_text.visible = false
about_menu_text:reparent(scene)

local demo_menu_back = create_node("text")
demo_menu_back.position = UIDim:new(nil, 0.5, 0, 0.6, 0)
demo_menu_back:font(font_path, 64)
demo_menu_back.text = "Back"
demo_menu_back.justify = "center"
demo_menu_back.visible = false
demo_menu_back:reparent(scene)

local demo_menu_osc = create_node("text")
demo_menu_osc.position = UIDim:new(nil, 0.5, 0, 0.3, 0)
demo_menu_osc:font(font_path, 64)
demo_menu_osc.text = "Oscilloscope"
demo_menu_osc.justify = "center"
demo_menu_osc.visible = false
demo_menu_osc:reparent(scene)

local demo_menu_stress = create_node("text")
demo_menu_stress.position = UIDim:new(nil, 0.5, 0, 0.4, 0)
demo_menu_stress:font(font_path, 64)
demo_menu_stress.text = "Stress Test"
demo_menu_stress.justify = "center"
demo_menu_stress.visible = false
demo_menu_stress:reparent(scene)

local demo_menu_puff = create_node("text")
demo_menu_puff.position = UIDim:new(nil, 0.5, 0, 0.5, 0)
demo_menu_puff:font(font_path, 64)
demo_menu_puff.text = "Puff"
demo_menu_puff.justify = "center"
demo_menu_puff.visible = false
demo_menu_puff:reparent(scene)

local osc_demo_prim = create_node("primitive")
osc_demo_prim:create_vertex_buffer(1024)
osc_demo_prim.ll_type = al_ffi.ALLEGRO_PRIM_POINT_LIST
osc_demo_prim:reparent(scene)

local osc_demo_explainer = create_node("text")
osc_demo_explainer.position = UIDim:new(nil, 0.5, 0, 0.15, 0)
osc_demo_explainer:font(sansfont_path, 32)
osc_demo_explainer.text = "Press S to pause audio. Press P to play."
osc_demo_explainer.justify = "center"
osc_demo_explainer.visible = false
osc_demo_explainer:reparent(scene)

local osc_demo_left_line = create_node("primitive")
local osc_demo_right_line = create_node("primitive")
do
    local left_x = (chrus.get_screen_width() / 2) - 512
    local right_x = (chrus.get_screen_width() / 2) + 512
    local up_y = (chrus.get_screen_height() / 2) - 150
    local down_y = (chrus.get_screen_height() / 2) + 150

    osc_demo_left_line:line(left_x, up_y, left_x, down_y, 2, green)
    osc_demo_right_line:line(right_x, up_y, right_x, down_y, 2, green)
    osc_demo_left_line.visible = false
    osc_demo_right_line.visible = false

    osc_demo_left_line:reparent(scene)
    osc_demo_right_line:reparent(scene)
end

chrus.set_window_title("Chrus Engine Demo -- Menu")
chrus.set_window_icon(stupid_icon)

local cap = 10000
local stress_sprites = {}
local rotations = {}

for i = 1, cap do
    stress_sprites[i] = create_node("sprite")
    stress_sprites[i].bitmap = cat_bitmap
    stress_sprites[i].x = math.random(50, 1920 - 100)
    stress_sprites[i].y = math.random(50, 1080 - 200)
    stress_sprites[i].visible = false
    stress_sprites[i]:reparent(scene)
    rotations[i] = (math.random() * 0.2) - 0.1
end

local stress_text = create_node("text")
stress_text.position = UIDim:new(nil, 0.5, 0, 0.15, 0)
stress_text:font(sansfont_path, 32)
stress_text.text = "There are 10,000 sprites rotating on the screen right now.\n\nPress Q to toggle the noise shader."
stress_text.justify = "center"
stress_text.visible = false
stress_text:reparent(scene)
stress_sprites[cap + 1] = stress_text

local puff_background = create_node("primitive")
puff_background:create_vertex_buffer(4)
do
    puff_background.vertices[0].x = 0
    puff_background.vertices[0].y = 0
    puff_background.vertices[0].color = Color.rgb(255, 0, 0)
    puff_background.vertices[1].x = 1920
    puff_background.vertices[1].y = 0
    puff_background.vertices[1].color = Color.rgb(0, 255, 0)
    puff_background.vertices[2].x = 0
    puff_background.vertices[2].y = 1080
    puff_background.vertices[2].color = Color.rgb(0, 0, 255)
    puff_background.vertices[3].x = 1920
    puff_background.vertices[3].y = 1080
    puff_background.vertices[3].color = Color.rgb(255, 255, 0)
end
puff_background.visible = false
puff_background.ll_type = al_ffi.ALLEGRO_PRIM_TRIANGLE_STRIP
puff_background:reparent(scene)

local puff_demo_sprite = create_node("sprite")
puff_demo_sprite.bitmap = puff_bitmap
puff_demo_sprite.x = (chrus.get_screen_width() - puff_demo_sprite.width) / 2
puff_demo_sprite.y = (chrus.get_screen_height() - puff_demo_sprite.height) / 2
puff_demo_sprite.visible = false
puff_demo_sprite:reparent(scene)

local puff_demo_helpertext = create_node("text")
puff_demo_helpertext.position = UIDim:new(nil, 0.5, 0, 0.15, 0)
puff_demo_helpertext:font(sansfont_path, 32)
puff_demo_helpertext.text = "Puff, my beloved"
puff_demo_helpertext.justify = "center"
puff_demo_helpertext.visible = false
puff_demo_helpertext:reparent(scene)

local puff_demo_helpertext2 = create_node("text")
puff_demo_helpertext2.position = UIDim:new(nil, 0.5, 0, 0.20, 0)
puff_demo_helpertext2:font(sansfont_path, 16)
puff_demo_helpertext2.text = "click to stretch puff"
puff_demo_helpertext2.justify = "center"
puff_demo_helpertext2.visible = false
puff_demo_helpertext2:reparent(scene)

local menu_objects = {welcome_menu_about, welcome_menu_demos, about_menu_text, 
    about_menu_back, about_menu_puff, demo_menu_back, demo_menu_osc, demo_menu_stress, demo_menu_puff}
local demo_objects = {osc_demo_prim, osc_demo_explainer, osc_demo_left_line, osc_demo_right_line, puff_demo_sprite, puff_background, puff_demo_helpertext, puff_demo_helpertext2}
local welcome_background = {background_rect, welcome_text, welcome_menu_frame}
local welcome_menu = {welcome_menu_about, welcome_menu_demos}
local about_menu = {about_menu_text, about_menu_back, about_menu_puff}
local demo_menu = {demo_menu_back, demo_menu_osc, demo_menu_stress, demo_menu_puff}

local osc_demo_objects = {osc_demo_prim, osc_demo_explainer, osc_demo_left_line, osc_demo_right_line}
local puff_demo_objects = {puff_demo_sprite, puff_background, puff_demo_helpertext, puff_demo_helpertext2}

local function hide_objects(tbl)
    for _, obj in pairs(tbl) do
        obj.visible = false
    end
end

local function show_objects(tbl)
    for _, obj in pairs(tbl) do
        obj.visible = true
    end
end

local function create_onhover_func(obj)
    return function(x, y)
        --if not obj.visible then return end

        if obj.x - obj.width <= x and x <= obj.x + obj.width and obj.y - obj.height <= y and y <= obj.y + obj.height * 2 then
            obj.color = Color.rgb(255, 255, 150)
        else
            obj.color = Color.rgb(255, 255, 255)
        end
    end
end

local function create_onclick_func(obj, callback)
    return function(x, y)
        if not obj.visible then return end

        if obj.x - obj.width <= x and x <= obj.x + obj.width and obj.y - obj.height <= y and y <= obj.y + obj.height * 2 then
            if callback then callback() end
            return true
        end
    end
end

local state = "menu"

local function write_oscilloscope()
    if state ~= "oscilloscope" then return end

    local audio_buffer = get_mixer_buffer()
    local vertex_buffer = osc_demo_prim.vertices
    local i
    for i = 0, 1023 do
        vertex_buffer[i].x = (chrus.get_screen_width() / 2) + (i - 512)
        vertex_buffer[i].y = (chrus.get_screen_height() / 2) + (audio_buffer[i] * 200)
        vertex_buffer[i].color = green
    end
end

local function rotate_sprites()
    if state ~= "stress" then return end

    for i = 1, cap do
        stress_sprites[i].rotation = stress_sprites[i].rotation + rotations[i]
    end
end

local states = {
    menu = function ()
        hide_objects(menu_objects)
        hide_objects(demo_objects)
        hide_objects(stress_sprites)
        show_objects(welcome_background)
        show_objects(welcome_menu)
        chrus.set_window_title("Chrus Engine Demo -- Menu")
        state = "menu"
    end,
    about = function ()
        hide_objects(menu_objects)
        show_objects(about_menu)
        chrus.set_window_title("Chrus Engine Demo -- About")
        state = "about"
    end,
    demos = function ()
        hide_objects(menu_objects)
        show_objects(demo_menu)
        chrus.set_window_title("Chrus Engine Demo -- Demos")
        state = "demos"
    end,
    oscilloscope = function ()
        hide_objects(menu_objects)
        hide_objects(welcome_background)
        chrus.set_window_title("Chrus Engine Demo -- Oscilloscope")
        state = "oscilloscope"
        show_objects(osc_demo_objects)
    end,
    stress = function ()
        hide_objects(menu_objects)
        hide_objects(welcome_background)
        chrus.set_window_title("Chrus Engine Demo -- Stress Test")
        state = "stress"
        show_objects(stress_sprites)
    end,
    puff = function ()
        hide_objects(menu_objects)
        hide_objects(welcome_background)
        chrus.set_window_title("Chrus Engine Demo -- Puff")
        state = "puff"
        show_objects(puff_demo_objects)
        puff_demo_sprite.sx = 1
    end,
}

local function stretch_puff()
    puff_demo_sprite.sx = puff_demo_sprite.sx + 0.05
end

get_mouse().moved:connect(create_onhover_func(welcome_menu_about))
get_mouse().moved:connect(create_onhover_func(about_menu_back))
get_mouse().moved:connect(create_onhover_func(welcome_menu_demos))
get_mouse().moved:connect(create_onhover_func(demo_menu_back))
get_mouse().moved:connect(create_onhover_func(demo_menu_osc))
get_mouse().moved:connect(create_onhover_func(demo_menu_stress))
get_mouse().moved:connect(create_onhover_func(demo_menu_puff))

get_mouse().clicked:connect(create_onclick_func(welcome_menu_about, states["about"]))
get_mouse().clicked:connect(create_onclick_func(about_menu_back, states["menu"]))
get_mouse().clicked:connect(create_onclick_func(welcome_menu_demos, states["demos"]))
get_mouse().clicked:connect(create_onclick_func(demo_menu_back, states["menu"]))
get_mouse().clicked:connect(create_onclick_func(demo_menu_osc, states["oscilloscope"]))
get_mouse().clicked:connect(create_onclick_func(demo_menu_stress, states["stress"]))
get_mouse().clicked:connect(create_onclick_func(demo_menu_puff, states["puff"]))
get_mouse().clicked:connect(create_onclick_func(puff_demo_sprite, stretch_puff))

get_tick().tick:connect(function ()
    about_menu_puff.rotation = about_menu_puff.rotation + 0.04
    write_oscilloscope()
    rotate_sprites()
end)

get_keyboard().keydown:connect(function(keycode)
    if keycode == al_ffi.ALLEGRO_KEY_ESCAPE then
        states["menu"]()
    end
end)

get_keyboard().keydown:connect(function(keycode)
    if state ~= "oscilloscope" then return end

    if keycode == al_ffi.ALLEGRO_KEY_S then
        sound:stop()
    elseif keycode == al_ffi.ALLEGRO_KEY_P then
        sound:play()
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