#include "Texture.h"

/**
 * @brief Creates a texture object with an assigned file path.
 *
 * Initializes:
 * - textureID = 0
 * - width/height/bitDepth = 0
 * - fileLocation = fileLoc
 *
 * @param fileLoc Path to the texture file.
 */
Texture::Texture(const char* fileLoc)
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = fileLoc;
}

/**
 * @brief Deletes the OpenGL texture and resets texture data.
 */
void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation.clear();
}

/**
 * @brief Destructor clears the texture.
 */
Texture::~Texture()
{
    ClearTexture();
}

/**
 * @brief Loads a texture (RGB or RGBA).  
 *
 * Internally calls LoadTextureAlpha().
 *
 * @return True if loaded successfully.
 */
bool Texture::LoadTexture()
{
    return LoadTextureAlpha();
}

/**
 * @brief Loads a texture with support for alpha channels.
 *
 * Supports:
 * - RGB (3 channels)
 * - RGBA (4 channels)
 *
 * Uses sRGB formats for correct lighting.
 *
 * @return True if loaded successfully.
 */
bool Texture::LoadTextureAlpha()
{
    printf("Loading texture with alpha: %s\n", fileLocation.c_str());
    fflush(stdout);

    unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("Failed to load texture: %s\n", fileLocation.c_str());
        fflush(stdout);
        return false;
    }

    GLenum internalFormat = 0;
    GLenum dataFormat = 0;

    if (bitDepth == 4) {
        internalFormat = GL_SRGB_ALPHA;
        dataFormat = GL_RGBA;
        printf("Texture has alpha channel.\n");
    }
    else if (bitDepth == 3) {
        internalFormat = GL_SRGB;
        dataFormat = GL_RGB;
    }
    else {
        printf("Texture format not supported (BitDepth: %d): %s\n", bitDepth, fileLocation.c_str());
        stbi_image_free(texData);
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                 width, height, 0, dataFormat, GL_UNSIGNED_BYTE, texData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texData);

    return true;
}

/**
 * @brief Loads normal maps or height/parallax maps.
 *
 * Supports:
 * - RGB (3 channels)
 * - RGBA (4 channels)
 * - RED (1 channel)
 *
 * @return True if loaded successfully.
 */
bool Texture::LoadMaps()
{
    printf("Loading map texture: %s\n", fileLocation.c_str());
    fflush(stdout);

    unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("Failed to load texture: %s\n", fileLocation.c_str());
        fflush(stdout);
        return false;
    }

    GLenum internalFormat = GL_RGB8;
    GLenum dataFormat = 0;

    if (bitDepth == 3)
        dataFormat = GL_RGB;
    else if (bitDepth == 4)
        dataFormat = GL_RGBA;
    else if (bitDepth == 1)
        dataFormat = GL_RED;
    else {
        printf("Unsupported normal map channel count: %d\n", bitDepth);
        stbi_image_free(texData);
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
                 width, height, 0, dataFormat, GL_UNSIGNED_BYTE, texData);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(texData);

    return true;
}

/**
 * @brief Binds the texture to a given texture unit.
 *
 * @param textureUnit GL texture unit (e.g., GL_TEXTURE0).
 */
void Texture::UseTexture(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
