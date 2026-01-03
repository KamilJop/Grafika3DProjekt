#include "TextRenderer.h"

/**
 * @brief Creates a text renderer with an orthographic projection.
 *
 * Initializes:
 * - Text shader
 * - Projection matrix
 * - VAO/VBO for rendering glyph quads
 */
TextRenderer::TextRenderer(unsigned int width, unsigned int height)
{
    textShader = new Shader();
    textShader->CreateShader("Shaders/textShader.vert", "Shaders/textShader.frag");

    glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

    textShader->UseShader();
    textShader->setMat4("projection", projection);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**
 * @brief Frees VAO/VBO and shader.
 */
TextRenderer::~TextRenderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    textShader->ClearShader();
    textShader = nullptr;
}

/**
 * @brief Loads a font using FreeType and generates glyph textures.
 *
 * Stores:
 * - Texture ID
 * - Size
 * - Bearing
 * - Advance
 *
 * @param font Path to font file.
 * @param fontSize Pixel height of glyphs.
 */
void TextRenderer::Load(std::string font, unsigned int fontSize)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font\n";
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };

        Characters.insert(std::pair<char, Character>(c, character));
    }

    printf("Loaded font: %s\n", font.c_str());
}

/**
 * @brief Renders a string of text at a given position.
 *
 * @param text The text to render.
 * @param x X position.
 * @param y Y position.
 * @param scale Text scale.
 * @param color RGBA color.
 */
void TextRenderer::RenderText(std::string text, float x, float y,
                              float scale, glm::vec4 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);

    textShader->UseShader();
    textShader->setVec4("textColor", color);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for (auto c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
}

/**
 * @brief Returns the width of a text string in pixels.
 */
float TextRenderer::GetTextWidth(std::string text)
{
    float width = 0.0f;

    for (auto c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];
        width += (ch.Advance >> 6);
    }

    return width;
}
