#include "Config.h"

void Config::Load() {
	std::ifstream configFile(configFilePath);
	if (!configFile.is_open()) {
		std::cout << "Could not open config file for reading: " << configFilePath << std::endl;
		return;
	}
	configFile >> screenWidth >> screenHeight;
	configFile >> musicVolume >> sfxVolume;
	configFile >> fullscreen >> showFPS >> antiAliasing;
	configFile >> outlineColor[0] >> outlineColor[1] >> outlineColor[2];
	configFile >> highlightColor[0] >> highlightColor[1] >> highlightColor[2];
	configFile.close();
}

void Config::Save() {
	std::ofstream configFile(configFilePath);
	if (!configFile.is_open()) {
		std::cout << "Could not open config file for writing: " << configFilePath << std::endl;
		return;
	}
	configFile << screenWidth << " " << screenHeight << std::endl;
	configFile << musicVolume << " " << sfxVolume << std::endl;
	configFile << fullscreen << " " << showFPS << " " << antiAliasing << std::endl;
	configFile << outlineColor[0] << " " << outlineColor[1] << " " << outlineColor[2] << std::endl;
	configFile << highlightColor[0] << " " << highlightColor[1] << " " << highlightColor[2] << std::endl;
	configFile.close();
}