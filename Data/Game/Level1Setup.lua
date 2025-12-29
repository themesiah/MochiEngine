-- Clear the events table
TimeMarkerEvents.Events = {}

local cloudGenerator = CloudGenerator:new()

TimeMarkerEvents.Events["IntroStart"] = function()
    cloudGenerator:Start()
    Wait(3)
    cloudGenerator:ForceNextForeground()
end

TimeMarkerEvents.Events["Level1-M1"] = function()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):Execute()
    Wait(2.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,3,0.7):WithShot(3, 0):Execute()
    Wait(0.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,-3,0.7):Execute()
    Wait(3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(1,3,0.1):WithShot(1, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-1,-3,0.1):WithShot(1, 0):Execute()
end

TimeMarkerEvents.Events["Level1-M2"] = function()
    EnemyAngel:new(CreateEnemy, 1):WithScale(3):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(15,0), 1, 3):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(3):WithDelay(0.2):WithAmount(10):WithDuration(5):WithYMovement(5,-5,0.5):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(3):WithDelay(0.2):WithAmount(10):WithDuration(5):WithYMovement(-5,5,0.5):Execute()
    Wait(4)
    
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,-5), 0, 0):WithPosition(Vector2f:new(12,-5), 2, 2):WithPosition(Vector2f:new(0,7), 2, 2):WithPosition(Vector2f:new(-18,7), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(-4,-4,0):WithShot(1, 0):Execute()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(-1,-1,0):WithShot(1, 0):Execute()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(-3,-3,0):WithShot(1, 0):Execute()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(5,5,0):WithShot(1, 0):Execute()
    Wait(0.5)
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,6), 0, 0):WithPosition(Vector2f:new(12,-5), 2, 2):WithPosition(Vector2f:new(0,-5), 2, 2):WithPosition(Vector2f:new(-18,-5), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(0,0,0):WithShot(1, 0):Execute()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithAmount(1):WithDuration(2):WithYMovement(-1,-1,0):WithShot(1, 0):Execute()
end

TimeMarkerEvents.Events["Level1-M3"] = function()
    -- Phase 1
    Wait(0.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(3,3,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(5,5,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(-2,-2,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(-6,-6,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(12,0), 1, 3):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()
    

    Wait(1)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(5,5,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(-7,-7,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(1,1,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(0,0,0):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(12,0), 1, 3):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()    
end

TimeMarkerEvents.Events["Level1-M4"] = function()
    Wait(0.4)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(3,5,0.5):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(5,7,0.5):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(-2,-4,0.5):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(3):WithDuration(5):WithYMovement(-6,-4,0.5):WithShot(1, 0):Execute()
    Wait(1.3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,7), 0, 0):WithPosition(Vector2f:new(12,7), 1, 3):WithPosition(Vector2f:new(-18,7), 3, 0):Execute()
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithHealth(30):WithPosition(Vector2f:new(18,-7), 0, 0):WithPosition(Vector2f:new(12,-7), 1, 3):WithPosition(Vector2f:new(-18,-7), 3, 0):Execute()
end

TimeMarkerEvents.Events["Level1-M5"] = function()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(4.5,-1.5,0.5):WithShot(3, 0):Execute()
    Wait(2.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-4.5,1.5,0.5):WithShot(3, 0):Execute()
    Wait(3.5)
    
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(5,-5,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3.5,-6.5,0.5):WithShot(3, 0):Execute()
    Wait(2.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-5,5,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3.5,6.5,0.5):WithShot(3, 0):Execute()
end

TimeMarkerEvents.Events["Level1-M6"] = function()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):Execute()
    Wait(2.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,3,0.7):WithShot(3, 0):Execute()
    Wait(0.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,-3,0.7):Execute()
    Wait(3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(3):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(15,0), 1, 3):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(1,3,0.1):WithShot(1, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-1,-3,0.1):WithShot(1, 0):Execute()
end

TimeMarkerEvents.Events["Level1-M7"] = function()
    Wait(0.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):Execute()
    Wait(2.5)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,3,0.7):WithShot(3, 0):Execute()
    Wait(0.3)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.6):WithAmount(3):WithDuration(5):WithYMovement(0,-3,0.7):Execute()
    Wait(3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(3):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(15,0), 1, 3):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(1,3,0.1):WithShot(1, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-1,-3,0.1):WithShot(1, 0):Execute()
end

TimeMarkerEvents.Events["Level1-BossIntro"] = function()
    StartCoroutine(function()
        for i=0, 6, 0.1 do
            cloudGenerator:ForceNextForeground()
            Wait(0.1)
        end
    end)
    
    Tween(function(t, dt)
        bg:SetXPosition(t)
    end,
    function()
        bg:SetXPosition(1)
    end,
    3)
    Wait(6)
    --EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(0,0,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(0.5,0.5,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(-0.5,-0.5,0):Execute();
    Wait(6)
    --EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(3,3,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(3.5,3.5,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(2.5,2.5,0):Execute();
    Wait(6)
    --EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(-2,-2,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(-2.5,-2.5,0):Execute();
    EnemyGroup:new(CreateEnemy, 0):WithScale(1):WithHealth(2):WithDelay(0.08):WithAmount(30):WithDuration(5):WithYMovement(-1.5,-1.5,0):Execute();
end

Audio_LoadAudio("Master")
Audio_PlayBGM("Level1BGM")

-- Audio_SkipToPosition(10000)
-- Audio_SkipToPosition(22000)
-- Audio_SkipToPosition(34000)
-- Audio_SkipToPosition(47000)
-- Audio_SkipToPosition(59000)
-- Audio_SkipToPosition(71000)
-- Audio_SkipToPosition(95000)
-- cloudGenerator:Start()

--Audio_SetMixerVolume("", 0)