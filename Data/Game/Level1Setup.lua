TimeMarkerEvents = {}

-- x: -16~+16
-- y: -9~+9

function IntroStart()
    local enemy1 = CreateEnemy()
    local enemy2 = CreateEnemy()
    local enemy3 = CreateEnemy()

    enemy1:SetPosition(Vector2f.new(15,50))
    enemy2:SetPosition(Vector2f.new(15,50))
    enemy3:SetPosition(Vector2f.new(15,50))
    enemy1:SetScale(2)
    enemy2:SetScale(2)
    enemy3:SetScale(2)

    Tween(
    function(t, dt)
        enemy1:SetPosition(Vector2f.new(Lerp(18,-24, t), 3))
        enemy2:SetPosition(Vector2f.new(Lerp(21,-21, t), 3))
        enemy3:SetPosition(Vector2f.new(Lerp(24,-18, t), 3))
    end,
    function()
        DeleteEnemy(enemy1)
        DeleteEnemy(enemy2)
        DeleteEnemy(enemy3)
    end, 5)
end



TimeMarkerEvents["IntroStart"] = IntroStart

Audio_LoadAudio("Master")
Audio_PlayBGM("Level1BGM")

Audio_SetMixerVolume("", 0)