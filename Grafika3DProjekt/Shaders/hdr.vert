/**
 * @file screen.vert
 * @brief Screen-space vertex shader
 */

#version 330 core

/** Vertex position */
layout (location = 0) in vec3 aPos;

/** Texture coordinates */
layout (location = 1) in vec2 aTexCoords;

/** Passed texture coordinates */
out vec2 TexCoords;

/**
 * @brief Main shader function
 */
void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}
