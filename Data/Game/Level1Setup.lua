-- Clear the events table
TimeMarkerEvents.Events = {}

TimeMarkerEvents.Events["IntroStart"] = function()

    EnemyGroup(CreateEnemy, 2, 3, -3, 5, 0.3, 5, 0.5)
    Wait(2)
    EnemyGroup(CreateEnemy, 2, -4, 4, 5, 0.3, 5, 0.5)
    Wait(3)
    EnemyGroup(CreateEnemy, 2, 0, 0, 5, 0.3, 5, 0)
end

Audio_LoadAudio("Master")
Audio_PlayBGM("Level1BGM")

Audio_SetMixerVolume("", 0)