WorldBoundsX = Vector2f.new(-16, 16)
WorldBoundsY = Vector2f.new(-9, 9)

TimeMarkerEvents = {
}

GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    if TimeMarkerEvents.Events[name] ~= nil then
        StartCoroutine(TimeMarkerEvents.Events[name])
    end
end)

function EnemyGroup(CreateFunction, scale, start_y, end_y, duration, delay, amount, sub_t)
    StartCoroutine(function()
        for i=1, amount, 1 do
            local enemy = CreateFunction()
            enemy:SetPosition(Vector2f.new(1000,1000))
            enemy:SetScale(scale)
            local sub_t_val = sub_t / 2
            
            Tween(
            function(t, dt)
                local new_x = Lerp(18, -18, t)
                local t2 = InverseLerp(sub_t_val, 1-sub_t_val, t)
                local new_y = Lerp(start_y, end_y, t2)
                enemy:SetPosition(Vector2f.new(new_x, new_y))
            end,
            function()
                DeleteEnemy(enemy)
            end, duration)

            if i == 1 then
                local timesToShot = 3
                local timesShot = 0
                local bulletPoolIndex = 0
                Tween(
                    function(t, dt)
                        local expectedShots = t / (1 / (timesToShot+1))
                        if timesShot < expectedShots and not enemy:IsDead() then
                            ShotBullet(bulletPoolIndex, enemy:GetPosition())
                            timesShot = timesShot + 1
                        end
                    end,
                    function()
                        -- Nothing
                    end, duration)
            end
            
            Wait(delay)
        end
    end)
end

-- Create here functions and types to manage enemies and such