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

class AudioManager
{
public:
	static AudioManager& GetInstance();
	void Init();	

	void LoadMusicTrack(const std::string& name, const std::string& filepath);
	void Load3DSoundEffect(const std::string& name, const std::string& filepath);
	void Load2DSoundEffect(const std::string& name, const std::string& filepath);

	int PlayMusicTrack(const std::string& name, float volume = 1.0f, bool loop = true);
	int Play3DSoundEffect(const std::string& name, const glm::vec3& position, float volume = 1.0f);
	int Play2DSoundEffect(const std::string& name, float volume = 1.0f);

	void SetListenerPosition(const glm::vec3& position);
	void UpdateMusicVolume(int handle, float volume);
	void UpdateSFXVolume(float volume);
	void Update3DAudio();

	void StopMusic();
	void Cleanup();

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

