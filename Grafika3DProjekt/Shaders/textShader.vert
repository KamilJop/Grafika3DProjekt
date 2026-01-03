/**
 * @file textShader.vert
 * @ingroup Shaders
 * @brief Vertex shader for rendering bitmap text.
 *
 * Transforms 2D text quad vertices using a projection matrix
 * and passes UV coordinates to the fragment shader.
 */

#version 330

layout (location = 0) in vec2 pos;       // Vertex position
layout (location = 1) in vec2 texCoord;  // Texture coordinates

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    // Transform text quad into clip space
    gl_Position = projection * vec4(pos, 0.0, 1.0);

    // Pass UV to fragment shader
    TexCoords = texCoord;
}
