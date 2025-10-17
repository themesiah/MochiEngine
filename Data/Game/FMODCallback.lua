GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    --LogInfo("This is another event that doesn't use the parameters")
    if TimeMarkerEvents[name] ~= nil then
        start_task(TimeMarkerEvents[name])
    end
end)