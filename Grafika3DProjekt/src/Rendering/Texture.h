#pragma once
#include <glad/glad.h>
#include "External/stb_loader.h"
#include <string>

/**
 * @class Texture
 * @brief Loads and manages 2D textures, normal maps, and height maps.
 *
 * Supports:
 * - Standard textures (RGB / RGBA)
 * - sRGB color space for correct lighting
 * - Normal maps and parallax/height maps
 * - Binding to any texture unit
 */
class Texture
{
public:

    /**
     * @brief Creates a texture object with a file path.
     *
     * @param fileLoc Path to the texture file.
     */
    Texture(const char* fileLoc);

    /** @brief Destructor clears the texture. */
    ~Texture();

    /**
     * @brief Loads a texture (RGB or RGBA).
     *
     * Internally calls LoadTextureAlpha().
     *
     * @return True if loaded successfully.
     */
    bool LoadTexture();

    /**
     * @brief Loads a texture with support for alpha channels.
     *
     * Uses sRGB formats for correct lighting.
     *
     * @return True if loaded successfully.
     */
    bool LoadTextureAlpha();

    /**
     * @brief Loads normal maps or height/parallax maps.
     *
     * Supports RGB, RGBA, and RED channel formats.
     *
     * @return True if loaded successfully.
     */
    bool LoadMaps();

    /**
     * @brief Binds the texture to a given texture unit.
     *
     * @param textureUnit GL texture unit (e.g., GL_TEXTURE0).
     */
    void UseTexture(GLenum textureUnit);

    /** @brief Deletes the texture and resets data. */
    void ClearTexture();

    /** @brief Returns the OpenGL texture ID. */
    GLuint getTextureID() { return textureID; }

    /** @brief Returns texture width in pixels. */
    int getWidth() { return width; }

    /** @brief Returns texture height in pixels. */
    int getHeight() { return height; }

private:
    GLuint textureID;       ///< OpenGL texture object ID.
    int width;              ///< Texture width.
    int height;             ///< Texture height.
    int bitDepth;           ///< Number of color channels.
    std::string fileLocation; ///< File path to the texture.
};
