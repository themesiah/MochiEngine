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
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

struct FrameData
{
    std::string Filename; // for debug purposes
    SDL_FRect Frame;
    SDL_FRect SpriteSourceSize;
    float Duration;
};

struct FrameTag
{
    std::string Name;
    int From;
    int To;
    std::string Direction;
    int Repeat;
    std::string UserData;
};

struct AnimationsData
{
    std::string TexturePath;
    SDL_FPoint Size;
    std::vector<FrameData> Frames;
    std::unordered_map<std::string, FrameTag> Animations;
};

#endif