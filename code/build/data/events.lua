--[[

Each scene gets their own thread. At startup, we pass each scene thread their scene pointer
and their event queue. 

We'll pass scripts to load to each thread, and they'll also be passed their event queues.

]]

local scene = ffi.cast("chrus_scene*", scene)
local thread = ffi.cast("ALLEGRO_THREAD*", thread) 
local event_queue = scene.event_queue

local event = {
    connections = {}
}

function event:fire(...)
    for _, v in pairs(self.connections) do
        v(...)
    end
end

function event:connect(func)
    table.insert(self.connections, func)
end

function event:new(o)
    o = o or {}   -- create object if user does not provide one
    setmetatable(o, self)
    self.__index = self
    o.connections = {}
    return o
end

local al_ffi = require("data/allegro_ffi")
local lallegro = ffi.load("allegro")

lallegro.al_register_event_source(event_queue, lallegro.al_get_keyboard_event_source());
lallegro.al_register_event_source(event_queue, lallegro.al_get_mouse_event_source());

--setmetatable(event, event_metatable)

local mouse = {clicked = event:new(), leftclicked = event:new(), rightclicked = event:new(), moved = event:new() }
local keyboard = {keydown = event:new()}
local tick = {tick = event:new()}

get_mouse = function()
    return mouse
end

get_keyboard = function()
    return keyboard
end

get_tick = function()
    return tick
end

sprite_connect_leftclicked = function(sprite, func)
    mouse.leftclicked:connect(function(x, y)
        if sprite.x < x and x < sprite.x + sprite.width and sprite.y < y and y < sprite.y + sprite.height then
            func()
        end
    end)
end

--[[
mouse.clicked:connect(function(x, y)
    print(string.format("I clicked at (%d, %d)!", x, y))
end)

mouse.leftclicked:connect(function(x, y)
    print(string.format("I LEFT clicked at (%d, %d)!", x, y))
end)

mouse.rightclicked:connect(function(x, y)
    print(string.format("I RIGHT clicked at (%d, %d)!", x, y))
end)
]]--

--keyboard.keydown:connect(function(keycode)
--    print(string.format("Cheesed to meet you, %d!", keycode))
--end)

-- TODO: fill out event params n stuff
-- TODO: need something to do in response to loading a script
local e = ffi.new("ALLEGRO_EVENT")

local loaded_scripts = {}

local event_responses = {
    [al_ffi.ALLEGRO_EVENT_KEY_DOWN] = function ()
        keyboard.keydown:fire(e.keyboard.keycode)
    end,
    [al_ffi.ALLEGRO_EVENT_MOUSE_BUTTON_DOWN] = function ()
        mouse.clicked:fire(e.mouse.x, e.mouse.y)
        if e.mouse.button == 1 then
            mouse.leftclicked:fire(e.mouse.x, e.mouse.y)
        else
            mouse.rightclicked:fire(e.mouse.x, e.mouse.y)
        end
    end,
    [al_ffi.ALLEGRO_EVENT_MOUSE_AXES] = function ()
        mouse.moved:fire(e.mouse.x, e.mouse.y)
    end,
    [al_ffi.ALLEGRO_EVENT_TIMER] = function ()
        tick.tick:fire(lallegro.al_get_time())
    end,
    [1667591283] = function ()
        local script = ffi.cast("chrus_node*", e.user.data1)
        local src = ffi.string(script:get_source())
        local err
        if src == nil then
            warn("loading an empty script...?")
            return
        else
            print(("loading %s"):format(src))
        end
        
        loaded_scripts[src], err = loadfile(src)
        if (err) then
            print(err)
        end

        local result, err = pcall(loaded_scripts[src])
        if result == false then
            print(err)
        end
    end
}

setmetatable(event_responses, {__index = function(t, k) t[k] = function() end return function () end end})

lallegro.al_start_timer(scene.tick_timer)

while true do
    if lallegro.al_get_thread_should_stop(thread) then
        break
    end
    lallegro.al_wait_for_event(event_queue, e)

    event_responses[e.type]()
end