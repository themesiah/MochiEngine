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
function EnemyGroup:new(createFunctionCallback, enemyType)
    local obj = {
        CreateFunction = createFunctionCallback,
        EnemyType = enemyType,
        Scale = 1,
        StartY = 0,
        EndY = 0,
        Duration = 5,
        Delay = 0.3,
        Amount = 1,
        MovementYT = 0,
        TimesToShot = 0,
        BulletPoolIndex = 0,
        Health = 10
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

function EnemyGroup:WithHealth(health)
    self.Health = health
    return self
end

function EnemyGroup:Execute()
    StartCoroutine(function()
        for i=1, self.Amount, 1 do
            local enemy = self.CreateFunction(self.EnemyType)
            enemy:GetTransform().Position = Vector2f.new(1000,1000)
            enemy:SetHealth(self.Health)
            enemy:GetTransform().Scale = self.Scale
            local sub_t_val = self.MovementYT / 2
            
            Tween(
            function(t, dt)
                local new_x = Lerp(18, -18, t)
                local t2 = InverseLerp(sub_t_val, 1-sub_t_val, t)
                local new_y = Lerp(self.StartY, self.EndY, t2)
                enemy:GetTransform().Position = Vector2f.new(new_x, new_y)
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
                            ShotBullet(bulletPoolIndex, enemy:GetTransform().Position)
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


EnemyAngel = {}
EnemyAngel.__index = EnemyAngel
function EnemyAngel:new(createFunctionCallback, enemyType)
    local obj = {
        CreateFunction = createFunctionCallback,
        EnemyType = enemyType,
        Scale = 1,
        Positions = {},
        Duration = 5,
        StopTime = 0,
        TimesToShot = 0,
        BulletPoolIndex = 0,
        DelayBeforeShot = 0,
        Health = 50
    }
    setmetatable(obj, self)
    self.__index = self
    return obj
end

function EnemyAngel:WithScale(scale)
    self.Scale = scale
    return self
end

function EnemyAngel:WithPosition(position, duration, stopTime)
    table.insert(self.Positions, {Pos = position, Duration = duration, StopTime = stopTime})
    return self
end

function EnemyAngel:WithHealth(health)
    self.Health = health
    return self
end

function EnemyAngel:Execute()
    StartCoroutine(function()
        local enemy = self.CreateFunction(self.EnemyType)
        enemy:GetTransform().Position = Vector2f.new(1000,1000)
        enemy:SetHealth(self.Health)
        enemy:GetTransform().Scale = self.Scale
        
        local lastPosition = self.Positions[1].Pos
        for i=2, #self.Positions, 1 do
            Tween(
            function(t, dt)
                local newPosition = self.Positions[i].Pos
                local new_x = Lerp(lastPosition.x, newPosition.x, t)
                local new_y = Lerp(lastPosition.y, newPosition.y, t)
                enemy:GetTransform().Position = Vector2f.new(new_x, new_y)

            end,
            function()
                lastPosition = self.Positions[i].Pos

                if i ~= #self.Positions then
                    Wait(1)
                    local bulletPoolIndex = 1
                    local delayBetweenShots = 0.2
                    local directions = {Vector2f:new(-2, 1), Vector2f:new(-3, 1), Vector2f:new(-1, 0), Vector2f:new(-3, -1), Vector2f:new(-2, -1)}

                    for j=1, #directions, 1 do
                        if not enemy:IsDead() then
                            local bulletIndex = ShotBullet(bulletPoolIndex, enemy:GetTransform().Position)
                            SetBulletDirection(bulletPoolIndex, bulletIndex, directions[j])
                            Wait(delayBetweenShots)
                        end
                    end
                    
                    for j=#directions, 1, -1 do
                        if not enemy:IsDead() then
                            local bulletIndex = ShotBullet(bulletPoolIndex, enemy:GetTransform().Position)
                            SetBulletDirection(bulletPoolIndex, bulletIndex, directions[j])
                            Wait(delayBetweenShots)
                        end
                    end
                end
            end, self.Positions[i].Duration)
            Wait(self.Positions[i].Duration + self.Positions[i].StopTime)
        end
        DeleteEnemy(enemy)
    end)
end