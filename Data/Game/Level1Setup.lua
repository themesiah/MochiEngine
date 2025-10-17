TimeMarkerEvents = {}

function IntroStart()
    enemy1 = CreateEnemy()
    enemy2 = CreateEnemy()
    enemy3 = CreateEnemy()

    enemy1:SetPosition(Vector2f.new(6,1))
    enemy2:SetPosition(Vector2f.new(7, -2))
    enemy3:SetPosition(Vector2f.new(3,3))
    enemy1:SetScale(2)
    enemy2:SetScale(2)
    enemy3:SetScale(2)

    Wait(2)
    DeleteEnemy(enemy1)

    Wait(5)
    DeleteEnemy(enemy3)

    Wait(3)
    DeleteEnemy(enemy2)
end

-- start_task(main)



TimeMarkerEvents["IntroStart"] = IntroStart