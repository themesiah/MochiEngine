-- Clear the events table
TimeMarkerEvents.Events = {}

TimeMarkerEvents.Events["IntroStart"] = function()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(4.5,-1.5,0.5):WithShot(3, 0):Execute()
    Wait(2)
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-4.5,1.5,0.5):WithShot(3, 0):Execute()
    
    Wait(3)
    EnemyAngel:new(CreateEnemy, 1):WithScale(2):WithPosition(Vector2f:new(18,0), 0, 0):WithPosition(Vector2f:new(15,0), 1, 5):WithPosition(Vector2f:new(-18,0), 2, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(1,1,0):WithShot(3, 0):Execute()
    EnemyGroup:new(CreateEnemy, 0):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-1,-1,0):WithShot(3, 0):Execute()

 
end

Audio_LoadAudio("Master")
Audio_PlayBGM("Level1BGM")

Audio_SetMixerVolume("", 0)