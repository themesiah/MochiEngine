WorldBoundsX = Vector2f.new(-16, 16)
WorldBoundsY = Vector2f.new(-9, 9)

TimeMarkerEvents = {
}

GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    if TimeMarkerEvents.Events[name] ~= nil then
        StartCoroutine(TimeMarkerEvents.Events[name])
    end
end)

function EnemyGroup(CreateFunction, scale, height, duration, delay, amount)
    StartCoroutine(function()
        for i=1, amount, 1 do
            local enemy = CreateFunction()
            enemy:SetPosition(Vector2f.new(1000,1000))
            enemy:SetScale(scale)
            
            Tween(
            function(t, dt)
                enemy:SetPosition(Vector2f.new(Lerp(18, -18, t), height))
            end,
            function()
                DeleteEnemy(enemy)
            end, duration)
            
            Wait(delay)
        end
    end)
end

-- Create here functions and types to manage enemies and such