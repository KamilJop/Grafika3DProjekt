#pragma once
#include <glad/glad.h>

/**
 * @class Mesh
 * @brief Represents a drawable mesh stored in GPU buffers.
 *
 * Provides:
 * - VAO, VBO, and EBO management
 * - Creation from vertex/index data
 * - Rendering via glDrawElements
 */
class Mesh
{
public:

    /** @brief Creates an empty mesh with uninitialized buffers. */
    Mesh();

    /**
     * @brief Creates the mesh from vertex and index data.
     *
     * @param vertices Pointer to vertex data.
     * @param indices Pointer to index data.
     * @param numOfVertices Number of vertices.
     * @param numOfIndices Number of indices.
     * @param floatsPerVertex Number of floats per vertex (stride).
     */
    void CreateMesh(GLfloat* vertices, unsigned int* indices,
                    unsigned int numOfVertices, unsigned int numOfIndices,
                    unsigned int floatsPerVertex);

    /** @brief Renders the mesh using glDrawElements. */
    void RenderMesh();

    /** @brief Deletes VAO, VBO, and EBO buffers. */
    void ClearMesh();

    /** @brief Destructor cleans up GPU resources. */
    ~Mesh();

private:
    GLuint VAO;        ///< Vertex Array Object.
    GLuint VBO;        ///< Vertex Buffer Object.
    GLuint EBO;        ///< Element Buffer Object.
    GLsizei indexCount;///< Number of indices to draw.
};
