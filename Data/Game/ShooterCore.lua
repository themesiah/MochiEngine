WorldBoundsX = Vector2f.new(-16, 16)
WorldBoundsY = Vector2f.new(-9, 9)

TimeMarkerEvents = {
}

GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    if TimeMarkerEvents.Events[name] ~= nil then
        StartCoroutine(TimeMarkerEvents.Events[name])
    end
end)

EnemyGroup = {}
EnemyGroup.__index = EnemyGroup
function EnemyGroup:new(createFunctionCallback)
    local obj = {
        CreateFunction = createFunctionCallback,
        Scale = 1,
        StartY = 0,
        EndY = 0,
        Duration = 5,
        Delay = 0.3,
        Amount = 1,
        MovementYT = 0,
        TimesToShot = 0,
        BulletPoolIndex = 0
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function EnemyGroup:WithScale(scale)
    self.Scale = scale
    return self
end

function EnemyGroup:WithYMovement(startY, endY, startOnT)
    self.StartY = startY
    self.EndY = endY
    self.MovementYT = startOnT
    return self
end

function EnemyGroup:WithDuration(duration)
    self.Duration = duration
    return self
end

function EnemyGroup:WithDelay(delay)
    self.Delay = delay
    return self
end

function EnemyGroup:WithAmount(amount)
    self.Amount = amount
    return self
end

function EnemyGroup:WithShot(timesToShot, bulletPool)
    self.TimesToShot = timesToShot
    self.BulletPoolIndex = bulletPool
    return self
end

function EnemyGroup:Execute()
    StartCoroutine(function()
        for i=1, self.Amount, 1 do
            local enemy = self:CreateFunction()
            enemy:SetPosition(Vector2f.new(1000,1000))
            enemy:SetScale(self.Scale)
            local sub_t_val = self.MovementYT / 2
            
            Tween(
            function(t, dt)
                local new_x = Lerp(18, -18, t)
                local t2 = InverseLerp(sub_t_val, 1-sub_t_val, t)
                local new_y = Lerp(self.StartY, self.EndY, t2)
                enemy:SetPosition(Vector2f.new(new_x, new_y))
            end,
            function()
                DeleteEnemy(enemy)
            end, self.Duration)

            if i == 1 then
                local timesToShot = self.TimesToShot
                local timesShot = 0
                local bulletPoolIndex = self.BulletPoolIndex
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
                    end, self.Duration)
            end
            
            Wait(self.Delay)
        end
    end)
end


-- function EnemyGroup(CreateFunction, scale, start_y, end_y, duration, delay, amount, sub_t)
--     StartCoroutine(function()
--         for i=1, amount, 1 do
--             local enemy = CreateFunction()
--             enemy:SetPosition(Vector2f.new(1000,1000))
--             enemy:SetScale(scale)
--             local sub_t_val = sub_t / 2
            
--             Tween(
--             function(t, dt)
--                 local new_x = Lerp(18, -18, t)
--                 local t2 = InverseLerp(sub_t_val, 1-sub_t_val, t)
--                 local new_y = Lerp(start_y, end_y, t2)
--                 enemy:SetPosition(Vector2f.new(new_x, new_y))
--             end,
--             function()
--                 DeleteEnemy(enemy)
--             end, duration)

--             if i == 1 then
--                 local timesToShot = 3
--                 local timesShot = 0
--                 local bulletPoolIndex = 0
--                 Tween(
--                     function(t, dt)
--                         local expectedShots = t / (1 / (timesToShot+1))
--                         if timesShot < expectedShots and not enemy:IsDead() then
--                             ShotBullet(bulletPoolIndex, enemy:GetPosition())
--                             timesShot = timesShot + 1
--                         end
--                     end,
--                     function()
--                         -- Nothing
--                     end, duration)
--             end
            
--             Wait(delay)
--         end
--     end)
-- end

-- Create here functions and types to manage enemies and such