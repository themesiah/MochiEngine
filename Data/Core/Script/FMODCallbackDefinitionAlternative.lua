GetEvent("AudioMarkerEvent"):addListener(function(name, time)
    LogInfo("This is another event that doesn't use the parameters")
end)