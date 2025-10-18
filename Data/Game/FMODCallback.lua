GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    --LogInfo("This is another event that doesn't use the parameters")
    if TimeMarkerEvents[name] ~= nil then
        StartCoroutine(TimeMarkerEvents[name], "Holaaa")
    end
end)