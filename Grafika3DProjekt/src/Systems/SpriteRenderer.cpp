#include "SpriteRenderer.h"

/**
 * @brief Creates a sprite renderer using the given shader.
 *
 * Initializes the quad used to draw all sprites.
 */
SpriteRenderer::SpriteRenderer(Shader& shader)
{
    this->rendererShader = &shader;
    this->initRenderData();
}

/**
 * @brief Destructor that frees the quad VAO.
 */
SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

/**
 * @brief Draws a textured 2D sprite.
 *
 * Applies translation, rotation, and scaling to the quad,
 * then renders it using the provided texture.
 *
 * @param texture Texture to draw.
 * @param position Screen/world position.
 * @param size Sprite size.
 * @param rotate Rotation in degrees.
 * @param color Tint color.
 */
void SpriteRenderer::DrawSprite(Texture* texture, glm::vec2 position,
                                glm::vec2 size, float rotate, glm::vec3 color)
{
    rendererShader->UseShader();

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    rendererShader->setMat4("model", model);
    rendererShader->setVec3("spriteColor", color);

    texture->UseTexture(GL_TEXTURE0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

/**
 * @brief Initializes the quad used for rendering sprites.
 *
 * Creates a VAO/VBO containing a 1×1 textured quad.
 */
void SpriteRenderer::initRenderData()
{
    GLuint VBO;
    GLfloat vertices[] = {
        // pos      // uv
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
