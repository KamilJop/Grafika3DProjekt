#pragma once
#include <iostream>
class Config
{
public:
	int screenWidth = 1280;
	int screenHeight = 720;

	float musicVolume = 0.5f;
	float sfxVolume = 0.5f;

	bool fullscreen = false;
	bool showFPS = true;
	bool antiAliasing = true;

	float outlineColor[3] = { 1.0f, 1.0f, 1.0f };
	float highlightColor[3] = { 1.0f, 1.0f, 0.0f };


	// Singleton instance accessor
	static Config& getInstance()
	{
		static Config instance;
		return instance;
	}

	// Delete copy and move constructors and assign operators
	Config(const Config&) = delete;
	Config(Config&&) = delete;
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) = delete;

protected:
	Config() = default;
};

