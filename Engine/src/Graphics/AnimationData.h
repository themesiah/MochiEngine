#ifndef HDEF_ANIMATIONDATA
#define HDEF_ANIMATIONDATA

/*
FrameData:
    string filename
    sdlrect frame
    sdlrect spritesourcesize
    float duration

AnimationData:
    string name
    int from
    int to
    string direction -> parse to enum {forward, backward, pingpong, pingpongbackward}
    string repeat -> parse to int, may be null
    string userdata

AnimationsData:
    vector<framedata> frames
    vector<animationdata> frameTags

*/

#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    struct FrameData
    {
        std::string Filename; // for debug purposes
        Rectf Frame;
        Rectf SpriteSourceSize;
        float Duration;
    };

    enum AnimationDirection
    {
        Forward,
        Backward,
        Pingpong,
        BackwardPingPong
    };

    struct FrameTag
    {
        std::string Name;
        int From;
        int To;
        AnimationDirection Direction;
        int Repeat;
        std::string UserData;
    };

    struct AnimationsData
    {
        std::filesystem::path TexturePath;
        Vector2f Size;
        std::vector<FrameData> Frames;
        std::unordered_map<std::string, FrameTag> Animations;
    };
}

#endif