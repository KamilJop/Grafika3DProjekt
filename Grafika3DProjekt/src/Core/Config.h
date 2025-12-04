#pragma once
#include <iostream>
#include <fstream>
#include <string>
class Config
{
public:
	int screenWidth;
	int screenHeight;

	float musicVolume;
	float sfxVolume;

	bool fullscreen;
	bool showFPS;
	bool antiAliasing;

	float outlineColor[3];
	float highlightColor[3];


	// Singleton instance accessor
	static Config& getInstance()
	{
		static Config instance;
		instance.Load();
		return instance;
	}

	// Delete copy and move constructors and assign operators
	Config(const Config&) = delete;
	Config(Config&&) = delete;
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) = delete;

	void Load();
	void Save();

protected:
	Config() = default;
	std::string configFilePath = "config.txt";
};

