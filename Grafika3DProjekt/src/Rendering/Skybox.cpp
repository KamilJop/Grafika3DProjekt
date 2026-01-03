#include "Skybox.h"

/**
 * @brief Creates a skybox from 6 texture faces.
 *
 * Steps:
 * - Loads skybox shader
 * - Loads 6 cube‑map textures
 * - Creates a cube mesh for rendering
 *
 * @param faces Vector of 6 file paths (order: +X, -X, +Y, -Y, +Z, -Z).
 */
Skybox::Skybox(std::vector<std::string> faces)
{
    // Initialize skybox shader
    skyboxShader = new Shader();
    skyboxShader->CreateShader("Shaders/skybox.vert", "Shaders/skybox.frag");

    // Create cube map texture
    glGenTextures(1, &skyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    int width, height, bitDepth;

    // Load each face of the cube map
    for (size_t i = 0; i < 6; i++)
    {
        unsigned char* texData = stbi_load(faces[i].c_str(), &width, &height, &bitDepth, 0);
        if (texData)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
            stbi_image_free(texData);
        }
        else
        {
            std::cout << "Failed to load skybox texture at path: " << faces[i] << std::endl;
            stbi_image_free(texData);
        }
    }

    // Texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Cube vertices and indices
    unsigned int skyboxIndices[] = {
        // front
        0, 1, 2, 2, 1, 3,
        // right
        2, 3, 5, 5, 3, 7,
        // back
        5, 7, 4, 4, 7, 6,
        // left
        4, 6, 0, 0, 6, 1,
        // top
        4, 0, 5, 5, 0, 2,
        // bottom
        1, 6, 3, 3, 6, 7
    };

    float skyboxVertices[] = {
        // 8 vertices, each with dummy attributes (unused)
        -1.0f,  1.0f, -1.0f, 0,0, 0,0,0,
        -1.0f, -1.0f, -1.0f, 0,0, 0,0,0,
         1.0f,  1.0f, -1.0f, 0,0, 0,0,0,
         1.0f, -1.0f, -1.0f, 0,0, 0,0,0,

        -1.0f,  1.0f,  1.0f, 0,0, 0,0,0,
         1.0f,  1.0f,  1.0f, 0,0, 0,0,0,
        -1.0f, -1.0f,  1.0f, 0,0, 0,0,0,
         1.0f, -1.0f,  1.0f, 0,0, 0,0,0
    };

    // Create skybox mesh
    skyboxMesh = new Mesh();
    skyboxMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36, 8);
}

/**
 * @brief Renders the skybox behind all scene geometry.
 *
 * Removes translation from the view matrix so the skybox appears infinitely far.
 *
 * @param viewMatrix Camera view matrix.
 * @param projectionMatrix Camera projection matrix.
 */
void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    glDepthMask(GL_FALSE);

    skyboxShader->UseShader();

    // Remove translation from view matrix
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(viewMatrix)));
    skyboxShader->setMat4("projection", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    skyboxMesh->RenderMesh();

    glDepthMask(GL_TRUE);
}

/**
 * @brief Destructor cleans up mesh, shader, and cube map texture.
 */
Skybox::~Skybox()
{
    skyboxMesh->ClearMesh();
    delete skyboxMesh;

    skyboxShader->ClearShader();
    delete skyboxShader;

    glDeleteTextures(1, &skyboxTextureID);
}
