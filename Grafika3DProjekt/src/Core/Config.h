#pragma once
#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief Manages game configuration settings.
 * 
 * This class handles reading and writing configuration settings
 * to a file (`config.txt`). It uses the Singleton pattern so
 * there is only one instance of the configuration in the program.
 */
class Config
{
public:
    /** @brief Screen width in pixels */
    int screenWidth;

    /** @brief Screen height in pixels */
    int screenHeight;

    /** @brief Music volume [0.0 - 1.0] */
    float musicVolume;

    /** @brief Sound effects volume [0.0 - 1.0] */
    float sfxVolume;

    /** @brief Fullscreen mode enabled/disabled */
    bool fullscreen;

    /** @brief Show FPS counter on screen */
    bool showFPS;

    /** @brief Enable or disable anti-aliasing */
    bool antiAliasing;

    /** @brief RGB color for outlines (0-1 range) */
    float outlineColor[3];

    /** @brief RGB color for highlights (0-1 range) */
    float highlightColor[3];

    /**
     * @brief Access the singleton instance of Config.
     * 
     * Automatically loads the configuration from the file when called.
     * 
     * @return Config& Reference to the single Config instance.
     */
    static Config& getInstance()
    {
        static Config instance;
        instance.Load();
        return instance;
    }

    // Delete copy and move constructors and assignment operators
    Config(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(const Config&) = delete;
    Config& operator=(Config&&) = delete;

    /**
     * @brief Loads the configuration from the config file.
     * 
     * If the file cannot be opened, a default configuration is created.
     */
    void Load();

    /**
     * @brief Saves the current configuration to the config file.
     */
    void Save();

protected:
    /** @brief Default constructor (protected for singleton pattern) */
    Config() = default;

    /**
     * @brief Creates a default configuration and writes it to the file.
     * 
     * Called if the config file does not exist or cannot be read.
     */
    void CreateDefaultConfig();

    /** @brief Path to the configuration file */
    std::string configFilePath = "config.txt";
};
