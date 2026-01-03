#include "Config.h"
#include <fstream>
#include <iostream>

/**
 * @brief Loads the configuration from the config file.
 * 
 * If the file cannot be opened, prints a message to the console
 * and creates a default configuration file.
 */
void Config::Load() {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cout << "Could not open config file " << configFilePath << std::endl;
        std::cout << "Creating default config file." << std::endl;
        CreateDefaultConfig();
        return;
    }

    // Read settings from the file
    configFile >> screenWidth >> screenHeight;
    configFile >> musicVolume >> sfxVolume;
    configFile >> fullscreen >> showFPS >> antiAliasing;
    configFile >> outlineColor[0] >> outlineColor[1] >> outlineColor[2];
    configFile >> highlightColor[0] >> highlightColor[1] >> highlightColor[2];
    configFile.close();
}

/**
 * @brief Saves the current configuration to the config file.
 * 
 * If the file cannot be opened for writing, prints an error message.
 */
void Config::Save() {
    std::ofstream configFile(configFilePath);
    if (!configFile.is_open()) {
        std::cout << "Could not open config file for writing: " << configFilePath << std::endl;
        return;
    }

    // Write settings to the file
    configFile << screenWidth << " " << screenHeight << std::endl;
    configFile << musicVolume << " " << sfxVolume << std::endl;
    configFile << fullscreen << " " << showFPS << " " << antiAliasing << std::endl;
    configFile << outlineColor[0] << " " << outlineColor[1] << " " << outlineColor[2] << std::endl;
    configFile << highlightColor[0] << " " << highlightColor[1] << " " << highlightColor[2] << std::endl;
    configFile.close();
}

/**
 * @brief Creates a default configuration and saves it to the config file.
 * 
 * Default settings:
 * - screenWidth = 2560, screenHeight = 1440
 * - musicVolume = 0.5, sfxVolume = 0.5
 * - fullscreen = false, showFPS = false, antiAliasing = true
 * - outlineColor = red (1,0,0), highlightColor = green (0,1,0)
 */
void Config::CreateDefaultConfig() {
    screenWidth = 2560;
    screenHeight = 1440;
    musicVolume = 0.5f;
    sfxVolume = 0.5f;
    fullscreen = false;
    showFPS = false;
    antiAliasing = true;
    outlineColor[0] = 1.0f; outlineColor[1] = 0.0f; outlineColor[2] = 0.0f;
    highlightColor[0] = 0.0f; highlightColor[1] = 1.0f; highlightColor[2] = 0.0f;
    Save();
}
