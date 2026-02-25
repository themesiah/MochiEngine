function Lerp(a, b, t)
    return (b-a)*t + a
end

function InverseLerp(a, b, val)
    return (val-a) / (b-a)
end