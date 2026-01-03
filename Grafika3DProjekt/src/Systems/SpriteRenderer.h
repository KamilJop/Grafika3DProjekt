#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"

/**
 * @class SpriteRenderer
 * @brief Renders 2D textured quads (sprites) using a given shader.
 */
class SpriteRenderer
{
public:
    /**
     * @brief Creates a sprite renderer using the provided shader.
     *
     * @param shader Shader used to draw all sprites.
     */
    SpriteRenderer(Shader& shader);

    /** @brief Destructor that frees the quad VAO. */
    ~SpriteRenderer();

    /**
     * @brief Draws a textured sprite.
     *
     * @param texture Texture to draw.
     * @param position Position of the sprite.
     * @param size Size of the sprite (default 10×10).
     * @param rotate Rotation in degrees.
     * @param color Tint color.
     */
    void DrawSprite(Texture* texture,
                    glm::vec2 position,
                    glm::vec2 size = glm::vec2(10, 10),
                    float rotate = 0.0f,
                    glm::vec3 color = glm::vec3(1.0f));

private:
    Shader* rendererShader; ///< Shader used for rendering.
    GLuint quadVAO;         ///< VAO for the quad mesh.

    /** @brief Initializes the quad used for rendering sprites. */
    void initRenderData();
};
