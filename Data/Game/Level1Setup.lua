function main()
    enemy1 = CreateSprite("Enemy1.png")
    enemy2 = CreateSprite("Enemy1.png")
    enemy3 = CreateSprite("Enemy1.png")

    enemy1:SetPosition(Vector2f.new(6,1))
    enemy2:SetPosition(Vector2f.new(7, -2))
    enemy3:SetPosition(Vector2f.new(3,3))
    enemy1:SetScale(2)
    enemy2:SetScale(3)

    Wait(2);
    DeleteSprite(enemy1)

    Wait(5);
    DeleteSprite(enemy3)

    Wait(3);
    DeleteSprite(enemy2)
end

start_task(main)