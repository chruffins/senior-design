print("Hello world!")
local sound = create_node("audiostream")

sound:load("data/snowpatrol.ogg")
sound:play()
sound:reparent(scene)

local sprites = {}
local cap = 300

for i = 1, cap do
    sprites[i] = create_node("sprite")
    sprites[i]:load("data/test.png")
    sprites[i]:move(math.random(600), math.random(600))
    sprites[i]:reparent(scene)
end

get_tick().tick:connect(function()
    for i = 1, cap do
        sprites[i]:move(math.random(-2, 2), math.random(-2, 2))
    end
end)

get_keyboard().keydown:connect(function()
    sound:stop()
end)