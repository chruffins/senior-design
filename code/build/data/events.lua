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

local event_metatable = {
    __index = {
        fire = function(this, ...)
            for _, v in pairs(this.connections) do
                v(...)
            end
        end,
        connect = function(this, func)
            table.insert(this.connections, func)
        end
    }
}

local al_ffi = require("data/allegro_ffi")
local lallegro = ffi.load("allegro")

local queue = lallegro.al_create_event_queue()
lallegro.al_register_event_source(queue, lallegro.al_get_keyboard_event_source());
lallegro.al_register_event_source(queue, lallegro.al_get_mouse_event_source());

--setmetatable(event, event_metatable)
local e = ffi.new("ALLEGRO_EVENT")

local mouse = {clicked = event:new(), leftclicked = event:new(), rightclicked = event:new() }
local keyboard = {keydown = event:new()}

--[[
mouse.clicked:connect(function(x, y)
    print(string.format("I clicked at (%d, %d)!", x, y))
end)
]]--

mouse.leftclicked:connect(function(x, y)
    print(string.format("I LEFT clicked at (%d, %d)!", x, y))
end)

mouse.rightclicked:connect(function(x, y)
    print(string.format("I RIGHT clicked at (%d, %d)!", x, y))
end)

keyboard.keydown:connect(function(keycode)
    print(string.format("Cheesed to meet you, %d!", keycode))
end)

--mouse.clicked:fire()
while true do
    lallegro.al_wait_for_event(queue, e)
    if (e.type == al_ffi.ALLEGRO_EVENT_KEY_DOWN) then
        keyboard.keydown:fire(e.keyboard.keycode)
    elseif e.type == al_ffi.ALLEGRO_EVENT_MOUSE_BUTTON_DOWN then
        mouse.clicked:fire(e.mouse.x, e.mouse.y)
        if e.mouse.button == 1 then
            mouse.leftclicked:fire(e.mouse.x, e.mouse.y)
        else
            mouse.rightclicked:fire(e.mouse.x, e.mouse.y)
        end
    end
end