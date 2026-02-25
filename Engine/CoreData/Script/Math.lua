function Lerp(a, b, t)
    return (b-a)*t + a
end

function InverseLerp(a, b, val)
    if (a == b) then return val end
    if (val < a) then return 0 end
    if (val > b) then return 1 end
    return (val-a) / (b-a)
end