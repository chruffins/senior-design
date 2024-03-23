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
]]

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

local shader_enabled = false
keyboard.keydown:connect(function(keycode)
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

--[[
local text = create_node("text")
text.text = "Die."
text:reparent(scene)
]]

--[[

get_tick().tick:connect(function()
    for i = 1, cap do
        sprites[i]:move(math.random(-2, 2), math.random(-2, 2))
    end
end)

get_keyboard().keydown:connect(function()
    sound:stop()
end)
]]