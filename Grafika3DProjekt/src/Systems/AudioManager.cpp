#include "AudioManager.h"

/**
 * @brief Returns the global AudioManager singleton instance.
 */
AudioManager& AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

/**
 * @brief Initializes the SoLoud audio engine.
 */
void AudioManager::Init()
{
    soloud.init();
}

/**
 * @brief Loads a streamed music track (WAV/OGG/MP3).
 *
 * @param name Identifier used to reference the track.
 * @param filepath Path to the audio file.
 */
void AudioManager::LoadMusicTrack(const std::string& name, const std::string& filepath)
{
    SoLoud::WavStream* music = new SoLoud::WavStream();
    music->load(filepath.c_str());
    musicTracks[name] = music;
}

/**
 * @brief Loads a 3D positional sound effect.
 *
 * @param name Identifier for the sound.
 * @param filepath Path to the audio file.
 */
void AudioManager::Load3DSoundEffect(const std::string& name, const std::string& filepath)
{
    SoLoud::Wav* sound = new SoLoud::Wav();
    sound->load(filepath.c_str());
    soundEffects[name] = sound;
}

/**
 * @brief Loads a 2D (non-positional) sound effect.
 *
 * @param name Identifier for the sound.
 * @param filepath Path to the audio file.
 */
void AudioManager::Load2DSoundEffect(const std::string& name, const std::string& filepath)
{
    SoLoud::Wav* sound = new SoLoud::Wav();
    sound->load(filepath.c_str());
    soundEffects[name] = sound;
}

/**
 * @brief Plays a music track, stopping any currently playing track.
 *
 * @param name Track identifier.
 * @param volume Playback volume.
 * @param loop Whether the track should loop.
 * @return Handle to the playing instance, or -1 on failure.
 */
int AudioManager::PlayMusicTrack(const std::string& name, float volume, bool loop)
{
    if (currentMusicHandle != -1)
        soloud.stop(currentMusicHandle);

    auto it = musicTracks.find(name);
    if (it != musicTracks.end())
    {
        currentMusicHandle = soloud.play(*it->second, volume);
        soloud.setLooping(currentMusicHandle, loop);
        return currentMusicHandle;
    }
    return -1;
}

/**
 * @brief Plays a 3D sound effect at a world position.
 *
 * @param name Sound identifier.
 * @param position 3D world position.
 * @param volume Playback volume.
 * @return Handle to the playing instance, or -1 on failure.
 */
int AudioManager::Play3DSoundEffect(const std::string& name, const glm::vec3& position, float volume)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        int handle = soloud.play3d(*it->second,
                                   position.x, position.y, position.z,
                                   0.0f, 0.0f, 0.0f,
                                   volume);

        soloud.set3dSourceAttenuation(handle, 1, 0.2f);
        return handle;
    }
    return -1;
}

/**
 * @brief Plays a 2D sound effect.  
 *
 * If the sound is already playing, the previous instance is stopped.
 *
 * @param name Sound identifier.
 * @param volume Playback volume.
 * @return Handle to the playing instance, or -1 on failure.
 */
int AudioManager::Play2DSoundEffect(const std::string& name, float volume)
{
    auto it = soundEffects.find(name);
    if (it != soundEffects.end())
    {
        if (active2DHandles.count(name))
            soloud.stop(active2DHandles[name]);

        int handle = soloud.play(*it->second, volume);
        active2DHandles[name] = handle;
        return handle;
    }
    return -1;
}

/**
 * @brief Sets the listener's 3D world position.
 *
 * @param position Listener position.
 */
void AudioManager::SetListenerPosition(const glm::vec3& position)
{
    soloud.set3dListenerPosition(position.x, position.y, position.z);
}

/**
 * @brief Updates the volume of a playing music track.
 *
 * @param handle Audio handle.
 * @param volume New volume.
 */
void AudioManager::UpdateMusicVolume(int handle, float volume)
{
    soloud.setVolume(handle, volume);
}

/**
 * @brief Updates 3D audio calculations (must be called each frame).
 */
void AudioManager::Update3DAudio()
{
    soloud.update3dAudio();
}

/**
 * @brief Updates the volume of all sound effects.
 *
 * @param volume New volume.
 */
void AudioManager::UpdateSFXVolume(float volume)
{
    for (auto& sfx : soundEffects)
        sfx.second->setVolume(volume);
}

/**
 * @brief Stops the currently playing music track.
 */
void AudioManager::StopMusic()
{
    if (currentMusicHandle != -1)
    {
        soloud.stop(currentMusicHandle);
        currentMusicHandle = -1;
    }
}

/**
 * @brief Frees all loaded audio resources and shuts down SoLoud.
 */
void AudioManager::Cleanup()
{
    for (auto& pair : soundEffects)
        delete pair.second;
    soundEffects.clear();

    for (auto& pair : musicTracks)
        delete pair.second;
    musicTracks.clear();

    soloud.deinit();
}
