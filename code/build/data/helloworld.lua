print("Hello world!")

local sound = create_node("audiostream")

sound:load("data/snowpatrol.ogg")
sound:play()
sound:reparent(scene)

local texts = {}
local sprites = {}
local cap = 200
local keyboard = get_keyboard()

local strings = {
    "Hello world!",
    "I love life!",
    "Why"
}

for i = 1, cap do
    texts[i] = create_node("text")
    texts[i].text = strings[math.random(3)]
    texts[i].x = math.random(600) + 100
    texts[i].y = math.random(600) + 100
    texts[i].color = lallegro.al_map_rgb(math.random(100) + 50, math.random(100) + 50, math.random(100) + 50)
    texts[i]:reparent(scene)
end

for i = 1, cap do
    sprites[i] = create_node("sprite")
    sprites[i]:load("data/test.png")
    sprites[i]:move(math.random(600), math.random(600))
    sprites[i]:reparent(scene)
end

keyboard.keydown:connect(function(keycode)
    if keycode == al_ffi.ALLEGRO_KEY_S then
        sound:stop()
    elseif keycode == al_ffi.ALLEGRO_KEY_P then
        sound:play()
    end
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