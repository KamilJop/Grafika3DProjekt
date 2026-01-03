/**
 * @file skybox.vert
 * @brief Vertex shader for rendering the skybox cube.
 *
 * Passes cube vertex direction to the fragment shader
 * and transforms the cube using view + projection matrices.
 */

#version 330

layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Pass direction vector to fragment shader
    TexCoords = pos;

    // Transform skybox cube
    gl_Position = projection * view * vec4(pos, 1.0);
}
