CoroutineSystem = {
    active = {}
}

function Yield()
    return coroutine.yield()
end

function Wait(seconds)
    local target = seconds or 0
    local elapsed = 0
    while elapsed < target do
        local dt = Yield()
        elapsed = elapsed + dt
    end
end

function WaitFor(predicate)
    while not predicate() do Yield() end
end

function Tween(update_callback, end_callback, time)
    local currentDt = 0
    local maxTime = time
    local t = 0
    local cor = function()
        while t < 1 do
            currentDt = Yield()
            time = time - currentDt
            t = 1 - time/maxTime;
            if t < 1 then
                update_callback(t, currentDt)
            else
                update_callback(1, currentDt)
                end_callback()
            end
        end
    end
    StartCoroutine(cor)
end

function StartCoroutine(func)
    local co = coroutine.create(func)
    table.insert(CoroutineSystem.active, co)
    return co
end

function UpdateCoroutines(dt)
    for i = #CoroutineSystem.active, 1, -1 do
        local co = CoroutineSystem.active[i]
        if coroutine.status(co) == "dead" then
            table.remove(CoroutineSystem.active, i)
        else
            local ok, err = coroutine.resume(co, dt)
            if not ok then
                LogError("Coroutine error:" .. err)
                table.remove(CoroutineSystem.active, i)
            elseif coroutine.status(co) == "dead" then
                table.remove(CoroutineSystem.active, i)
            end
        end
    end
end

LogOk("Lua coroutines system initialized")