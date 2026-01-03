#include "Mesh.h"

/**
 * @brief Creates an empty mesh with uninitialized buffers.
 */
Mesh::Mesh()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indexCount = 0;
}

/**
 * @brief Deletes VAO, VBO, and EBO buffers.
 *
 * Called by the destructor to clean up GPU resources.
 */
void Mesh::ClearMesh()
{
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    indexCount = 0;
}

/**
 * @brief Destructor cleans up all mesh buffers.
 */
Mesh::~Mesh()
{
    ClearMesh();
}

/**
 * @brief Creates a mesh from vertex and index data.
 *
 * Layout (default floatsPerVertex = 8):
 * - Position: 3 floats
 * - Normal:   3 floats
 * - UV:       2 floats
 * - Tangent:  3 floats (optional)
 * - Bitangent:3 floats (optional)
 *
 * @param vertices Pointer to vertex data.
 * @param indices Pointer to index data.
 * @param numOfVertices Number of vertices.
 * @param numOfIndices Number of indices.
 * @param floatsPerVertex Number of floats per vertex.
 */
void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices,
                      unsigned int numOfVertices, unsigned int numOfIndices,
                      unsigned int floatsPerVertex)
{
    indexCount = numOfIndices;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices[0]) * numOfVertices,
                 vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices[0]) * numOfIndices,
                 indices, GL_STATIC_DRAW);

    GLsizei stride = floatsPerVertex * sizeof(GLfloat);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                          (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Tangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride,
                          (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // Bitangent
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride,
                          (GLvoid*)(11 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);

    glBindVertexArray(0);
}

/**
 * @brief Renders the mesh using glDrawElements.
 */
void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
