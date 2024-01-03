local sound = create_node("sound")

sound:load("data/snowpatrol.ogg")
sound:play()
sound:reparent(scene)

local sprites = {}

for i = 1, 50 do
    sprites[i] = create_node("sprite")
    sprites[i]:move(math.random(600), math.random(600))
    sprites[i]:reparent(scene)
end

for j = 1, 20 do
    for i = 1, 50 do
        sprites[i]:move(math.random(-20, 20), math.random(-20, 20))
    end
end