-- Clear the events table
TimeMarkerEvents.Events = {}

TimeMarkerEvents.Events["IntroStart"] = function()
    EnemyGroup:new(CreateEnemy):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(3,-3,0.5):WithShot(3, 0):Execute()
    Wait(2)
    EnemyGroup:new(CreateEnemy):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithYMovement(-3,3,0.5):WithShot(3, 0):Execute()
    Wait(3)
    EnemyGroup:new(CreateEnemy):WithScale(2):WithDelay(0.3):WithAmount(5):WithDuration(5):WithShot(3, 0):Execute()
end

Audio_LoadAudio("Master")
Audio_PlayBGM("Level1BGM")

Audio_SetMixerVolume("", 0)