#pragma once
#include "External/stb_loader.h"
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"

/**
 * @class Skybox
 * @brief Renders a cube‑map skybox around the scene.
 *
 * Handles:
 * - Loading 6 cube‑map textures
 * - Creating a cube mesh
 * - Rendering the skybox behind all geometry
 */
class Skybox
{
public:

    /**
     * @brief Creates a skybox from 6 texture file paths.
     *
     * @param faces Vector of 6 texture paths in cube‑map order.
     */
    Skybox(std::vector<std::string> faces);

    /**
     * @brief Draws the skybox using the given view and projection matrices.
     *
     * Removes translation from the view matrix so the skybox appears infinitely far.
     *
     * @param viewMatrix Camera view matrix.
     * @param projectionMatrix Camera projection matrix.
     */
    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    /** @brief Destructor cleans up mesh, shader, and cube‑map texture. */
    ~Skybox();

private:
    Mesh* skyboxMesh;        ///< Cube mesh used for rendering.
    Shader* skyboxShader;    ///< Shader used to draw the skybox.
    GLuint skyboxTextureID;  ///< OpenGL cube‑map texture ID.
};
