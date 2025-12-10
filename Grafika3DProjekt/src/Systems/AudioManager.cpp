#include "AudioManager.h"

AudioManager& AudioManager::GetInstance()
{
	static AudioManager instance;
	return instance;
}

void AudioManager::Init()
{
	soloud.init();
}

void AudioManager::LoadMusicTrack(const std::string& name, const std::string& filepath)
{
	SoLoud::WavStream* music = new SoLoud::WavStream();
	music->load(filepath.c_str());
	musicTracks[name] = music;
}

void AudioManager::Load3DSoundEffect(const std::string& name, const std::string& filepath)
{
	SoLoud::Wav* sound = new SoLoud::Wav();
	sound->load(filepath.c_str());
	soundEffects[name] = sound;
}

void AudioManager::Load2DSoundEffect(const std::string& name, const std::string& filepath)
{
	SoLoud::Wav* sound = new SoLoud::Wav();
	sound->load(filepath.c_str());
	soundEffects[name] = sound;
}

int AudioManager::PlayMusicTrack(const std::string& name, float volume, bool loop)
{
	if(currentMusicHandle != -1)
	{
		soloud.stop(currentMusicHandle);
	}
	auto it = musicTracks.find(name);
	if(it != musicTracks.end())
	{
		currentMusicHandle = soloud.play(*it->second, volume);
		soloud.setLooping(currentMusicHandle, loop);
		return currentMusicHandle;
	}
	return -1;
}

int AudioManager::Play3DSoundEffect(const std::string& name, const glm::vec3& position, float volume)
{
	auto it = soundEffects.find(name);
	if(it != soundEffects.end())
	{
		int handle = soloud.play3d(*it->second, position.x, position.y, position.z, 0.0f, 0.0f, 0.0f, volume);
		soloud.set3dSourceAttenuation(handle,1, 0.2f);
		return handle;
	}
	return -1;
}

int AudioManager::Play2DSoundEffect(const std::string& name, float volume)
{
	auto it = soundEffects.find(name);
	if(it != soundEffects.end())
	{
		if (active2DHandles.count(name))
		{
			soloud.stop(active2DHandles[name]);
		}
		int handle = soloud.play(*it->second, volume);
		active2DHandles[name] = handle;
		return handle;
	}
	return -1;
}

void AudioManager::SetListenerPosition(const glm::vec3& position)
{
	soloud.set3dListenerPosition(position.x, position.y, position.z);
}

void AudioManager::UpdateMusicVolume(int handle, float volume)
{
	soloud.setVolume(handle, volume);
}

void AudioManager::Update3DAudio()
{
	soloud.update3dAudio();
}

void AudioManager::UpdateSFXVolume(float volume) {
	for (auto &sfx : soundEffects)
	{
		sfx.second->setVolume(volume);
	}
}

void AudioManager::StopMusic()
{
	if(currentMusicHandle != -1)
	{
		soloud.stop(currentMusicHandle);
		currentMusicHandle = -1;
	}
}

void AudioManager::Cleanup()
{
	for(auto& pair : soundEffects)
	{
		delete pair.second;
	}
	soundEffects.clear();
	for(auto& pair : musicTracks)
	{
		delete pair.second;
	}
	musicTracks.clear();
	soloud.deinit();
}