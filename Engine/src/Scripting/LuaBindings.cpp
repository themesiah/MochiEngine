#include "LuaBindings.h"

#include "ScriptingManager.h"
#include <sol/sol.hpp>

#include "../Graphics/SpriteBase.h"
#include "../Graphics/Spritesheet.h"
#include "../Graphics/AnimatedSprite.h"
#include "../Graphics/OneshotAnimation.h"
#include "../Graphics/Camera.h"

#include "../Input/IActionManager.h"
#include "../Types/Types.hpp"
#include "../Time/TimeSystem.h"

#include "../Utils/Logger.h"

#include "../Audio/IAudioManager.h"

namespace Mochi::Scripting::Bindings
{
    void BindGraphics(ScriptingManager *manager, Graphics::Camera *camera)
    {
        manager->State.new_usertype<Graphics::SpriteBase>("SpriteBase",
                                                          "GetPosition", &Graphics::SpriteBase::GetPosition,
                                                          "SetPosition", &Graphics::SpriteBase::SetPosition,
                                                          "Move", &Graphics::SpriteBase::Move,
                                                          "GetScale", &Graphics::SpriteBase::GetScale,
                                                          "SetScale", &Graphics::SpriteBase::SetScale,
                                                          "GetZIndex", &Graphics::SpriteBase::GetZIndex,
                                                          "SetZIndex", &Graphics::SpriteBase::SetZIndex);

        manager->State.new_usertype<Graphics::Spritesheet>("Spritesheet",
                                                           sol::base_classes, sol::bases<Graphics::SpriteBase>(),
                                                           "SetFrame", &Graphics::Spritesheet::SetFrame,
                                                           "GetFrame", &Graphics::Spritesheet::GetFrame);

        manager->State.new_usertype<Graphics::AnimatedSprite>("AnimatedSprite",
                                                              sol::base_classes, sol::bases<Graphics::SpriteBase, Graphics::Spritesheet>(),
                                                              "PlayAnimation", &Graphics::AnimatedSprite::PlayAnimation,
                                                              "StopAnimation", &Graphics::AnimatedSprite::StopAnimation,
                                                              "GetCurrentAnimation", &Graphics::AnimatedSprite::GetCurrentAnimation);

        manager->State.new_usertype<Graphics::OneshotAnimation>("OneshotAnimation",
                                                                sol::base_classes, sol::bases<Graphics::SpriteBase, Graphics::Spritesheet, Graphics::AnimatedSprite>(),
                                                                "SetFinishCallback", &Graphics::OneshotAnimation::SetFinishCallback);

        manager->State.new_usertype<Graphics::Camera>("Camera",
                                                      "GetZoom", &Graphics::Camera::GetZoom,
                                                      "SetZoom", &Graphics::Camera::SetZoom,
                                                      "GetPosition", &Graphics::Camera::GetPosition,
                                                      "SetPosition", sol::overload(static_cast<void (Graphics::Camera::*)(const Vector2f &)>(&Graphics::Camera::SetPosition), static_cast<void (Graphics::Camera::*)(const float &, const float &)>(&Graphics::Camera::SetPosition)));

        manager->State["Camera"] = camera;

        LOG_OK("LUA Graphics methods and classes Binded");
    }

    void BindLogic(ScriptingManager *manager, Input::IActionManager *actionManager)
    {
        manager->State.new_usertype<Vector2f>(
            "Vector2f",
            sol::constructors<Vector2f(), Vector2f(float, float)>(),
            "x", &Vector2f::x,
            "y", &Vector2f::y,
            "Distance", &Vector2f::Distance,
            "Normalized", &Vector2f::Normalized,
            "Normalize", &Vector2f::Normalize,
            "Dot", sol::resolve<float(const Vector2f &) const>(&Vector2f::Dot),
            "CosineProximity", sol::resolve<float(const Vector2f &) const>(&Vector2f::CosineProximity),
            sol::meta_function::addition, ([](const Vector2f &a, const Vector2f &b)
                                           { return a + b; }),
            sol::meta_function::subtraction, ([](const Vector2f &a, const Vector2f &b)
                                              { return a - b; }),
            sol::meta_function::multiplication, sol::overload(([](const Vector2f &a, const Vector2f &b)
                                                               { return a * b; }),
                                                              ([](const Vector2f &a, const float &b)
                                                               { return a * b; })),
            sol::meta_function::division, sol::overload(([](const Vector2f &a, const Vector2f &b)
                                                         { return a / b; }),
                                                        ([](const Vector2f &a, const float &b)
                                                         { return a / b; })));

        manager->State.set_function("Action_Performed", [actionManager](const std::string &actionName)
                                    { return actionManager->Performed(actionName); });
        manager->State.set_function("Action_Value", [actionManager](const std::string &actionName)
                                    { return actionManager->Value(actionName); });
        manager->State.set_function("Action_CompoundValue", [actionManager](const std::string &actionName1, const std::string &actionName2)
                                    { return actionManager->CompoundValue(actionName1, actionName2); });

        manager->State.set_function("GetDeltaTime", []()
                                    { return Time::TimeSystem::GetDeltaTime(); });
        manager->State.set_function("GetUnscaledDeltaTime", []()
                                    { return Time::TimeSystem::GetUnscaledDeltaTime(); });
        manager->State.set_function("GetGameTime", []()
                                    { return Time::TimeSystem::GetGameTime(); });
        manager->State.set_function("GetUnscaledGameTime", []()
                                    { return Time::TimeSystem::GetUnscaledGameTime(); });
        LOG_OK("LUA Logic methods and classes Binded");
    }

    void BindAudio(ScriptingManager *manager, Audio::IAudioManager *audioManager)
    {
        manager->State.set_function("Audio_LoadAudio", [audioManager](const std::string &path)
                                    { audioManager->LoadAudio(path); });
        manager->State.set_function("Audio_UnloadAudio", [audioManager](const std::string &path)
                                    { audioManager->UnloadAudio(path); });
        manager->State.set_function("Audio_PlayBGM", [audioManager](const std::string &audioName)
                                    { audioManager->PlayBGM(audioName); });
        manager->State.set_function("Audio_PauseBGM", [audioManager]()
                                    { audioManager->PauseBGM(); });
        manager->State.set_function("Audio_StopBGM", [audioManager]()
                                    { audioManager->StopBGM(); });
        manager->State.set_function("Audio_ResumeBGM", [audioManager]()
                                    { audioManager->ResumeBGM(); });
        manager->State.set_function("Audio_SkipToPosition", [audioManager](const int &ms)
                                    { audioManager->SkipToTimelinePosition(ms); });
        manager->State.set_function("Audio_PlayOneshot", [audioManager](const std::string &audioName)
                                    { audioManager->PlayOneShot(audioName); });
        manager->State.set_function("Audio_SetParameter", [audioManager](const std::string &parameterName, const float &value)
                                    { audioManager->SetParameter(parameterName, value); });
        manager->State.set_function("Audio_SetMixerVolume", [audioManager](const std::string &mixerName, const float &value)
                                    { audioManager->SetMixerVolume(mixerName, value); });
        LOG_OK("LUA Audio methods and classes Binded");
    }
}