-- Define an Event "class"
Event = {}
Event.__index = Event

function Event:new()
    return setmetatable({ listeners = {} }, self)
end

function Event:addListener(func)
    table.insert(self.listeners, func)
end

function Event:fire(...)
    for _, listener in ipairs(self.listeners) do
        listener(...)
    end
end

Events = {}

function GetEvent(name)
    if not Events[name] then
        Events[name] = Event:new()
    end
    return Events[name]
end

function Wait(s)
    coroutine.yield(s)
end


LogOk("Lua starting scripts initialized")