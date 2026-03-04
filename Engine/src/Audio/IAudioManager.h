#ifndef HDEF_IAUDIOMANAGER
#define HDEF_IAUDIOMANAGER

#include <string>

namespace Mochi::Audio
{
    /**
     * @brief Interface for audio management, including loading and unloading audio, playing it, changing volume and other parameters.
     *
     * The implementations are meant to wrap some existing audio system to be used the same within the engine.
     */
    class IAudioManager
    {
    public:
        virtual ~IAudioManager() {};
        /// @brief Update internal audio subsystems.
        /// @param dt Delta time
        virtual void Update(const float &dt) = 0;
        /// @brief Preloads an audio file
        /// @param path The path to the file in the catalog
        virtual void LoadAudio(const std::string &path) = 0;
        /// @brief Unloads a loaded audio
        /// @param path The path to the file in the catalog
        virtual void UnloadAudio(const std::string &path) = 0;
        /// @brief Plays an audio as BGM
        /// @param audioName Identifier of the audio
        virtual void PlayBGM(const std::string &audioName) = 0;
        /// @brief Pauses current playing BGM (can resume at the same point)
        virtual void PauseBGM() = 0;
        /// @brief Stops current playing BGM (can't resume)
        virtual void StopBGM() = 0;
        /// @brief Resume current playing BGM
        virtual void ResumeBGM() = 0;
        /// @brief Skip current playing BGM to a certain position
        /// @param ms Point of the song in milliseconds
        virtual void SkipToTimelinePosition(const int &ms) = 0;
        /// @brief Plays an audio once and then stops it
        /// @param audioName Identifier of the audio
        virtual void PlayOneShot(const std::string &audioName) = 0;
        /// @brief Sets a parameter used by the audio system. This can vary between implementations. Check specific implementations of this class
        /// to check what you can do with this.
        /// @param parameterName The name or identifier of the parameter.
        /// @param value The value of the parameter.
        virtual void SetParameter(const std::string &parameterName, const float &value) = 0;
        /// @brief Changes the volume of a specific mixer/group.
        /// @param mixerGroupName The name of the mixer/group.
        /// @param value The new volume (between 0 and 1)
        virtual void SetMixerVolume(const std::string &mixerGroupName, const float &value) = 0;
    };
}

#endif