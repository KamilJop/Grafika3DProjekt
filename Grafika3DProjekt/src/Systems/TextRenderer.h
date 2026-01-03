#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <glad/glad.h>
#include "Rendering/Shader.h"

/**
 * @brief Stores data for a single font character.
 */
struct Character {
    unsigned int TextureID;   ///< Glyph texture.
    glm::ivec2   Size;        ///< Size of glyph.
    glm::ivec2   Bearing;     ///< Offset from baseline.
    unsigned int Advance;     ///< Horizontal offset to next glyph.
};

/**
 * @class TextRenderer
 * @brief Renders 2D text using FreeType and OpenGL.
 */
class TextRenderer
{
public:
    /**
     * @brief Creates a text renderer with an orthographic projection.
     *
     * @param width Screen width.
     * @param height Screen height.
     */
    TextRenderer(unsigned int width, unsigned int height);

    /** @brief Frees buffers and shader. */
    ~TextRenderer();

    /**
     * @brief Loads a font and generates glyph textures.
     *
     * @param font Path to font file.
     * @param fontSize Pixel height of characters.
     */
    void Load(std::string font, unsigned int fontSize);

    /**
     * @brief Renders text at a given position.
     *
     * @param text String to draw.
     * @param x X position.
     * @param y Y position.
     * @param scale Text scale.
     * @param color RGBA color.
     */
    void RenderText(std::string text, float x, float y, float scale, glm::vec4 color);

    /**
     * @brief Returns the width of a text string in pixels.
     */
    float GetTextWidth(std::string text);

private:
    unsigned int VAO, VBO;                 ///< Buffers for rendering quads.
    std::map<char, Character> Characters;  ///< Loaded glyphs.
    Shader* textShader;                    ///< Shader used for text rendering.
};
