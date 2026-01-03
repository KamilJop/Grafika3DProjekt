#pragma once
#define NOMINMAX
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "glm/glm.hpp"

#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>

/**
 * @class AudioManager
 * @brief Handles loading and playing 2D/3D sounds and music.
 *
 * Singleton class using SoLoud for audio playback.
 */
class AudioManager
{
public:
    /** @brief Returns the global audio manager instance. */
    static AudioManager& GetInstance();

    /** @brief Initializes the audio engine. */
    void Init();

    /** @brief Loads a streamed music track. */
    void LoadMusicTrack(const std::string& name, const std::string& filepath);

    /** @brief Loads a 3D sound effect. */
    void Load3DSoundEffect(const std::string& name, const std::string& filepath);

    /** @brief Loads a 2D sound effect. */
    void Load2DSoundEffect(const std::string& name, const std::string& filepath);

    /** @brief Plays a music track. */
    int PlayMusicTrack(const std::string& name, float volume = 1.0f, bool loop = true);

    /** @brief Plays a 3D sound at a position. */
    int Play3DSoundEffect(const std::string& name, const glm::vec3& position, float volume = 1.0f);

    /** @brief Plays a 2D sound. */
    int Play2DSoundEffect(const std::string& name, float volume = 1.0f);

    /** @brief Sets the listener's world position. */
    void SetListenerPosition(const glm::vec3& position);

    /** @brief Changes volume of a playing music track. */
    void UpdateMusicVolume(int handle, float volume);

    /** @brief Sets volume for all sound effects. */
    void UpdateSFXVolume(float volume);

    /** @brief Updates 3D audio (call every frame). */
    void Update3DAudio();

    /** @brief Stops currently playing music. */
    void StopMusic();

    /** @brief Frees all loaded audio and shuts down engine. */
    void Cleanup();

    // Disable copying/moving
    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

private:
    AudioManager() = default;

    SoLoud::Soloud soloud;
    std::map<std::string, SoLoud::Wav*> soundEffects;
    std::map<std::string, SoLoud::WavStream*> musicTracks;
    std::map<std::string, int> active2DHandles;

    int currentMusicHandle = -1;
};
