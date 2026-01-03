/**
 * @file sprite.vert
 * @brief Vertex shader for rendering 2D sprites.
 *
 * Takes a quad vertex (pos + UV packed into a vec4),
 * extracts texture coordinates, and transforms the quad
 * using model and projection matrices.
 */

#version 330 core

layout (location = 0) in vec4 vertex;   // xy = position, zw = UV

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    // Pass UV to fragment shader
    TexCoords = vertex.zw;

    // Transform sprite quad
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}
