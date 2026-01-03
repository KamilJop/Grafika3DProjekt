/**
 * @file outline.vert
 * @brief Vertex shader for rendering object outlines
 */

#version 330 core

/** Vertex position */
layout (location = 0) in vec3 aPos;

/** Vertex normal */
layout (location = 1) in vec3 aNormal;

/** Model transformation matrix */
uniform mat4 model;

/** View matrix */
uniform mat4 view;

/** Projection matrix */
uniform mat4 projection;

/** Outline thickness */
uniform float outline;

/**
 * @brief Main shader function
 *
 * Moves vertices along their normals by the outline value to create an outline effect.
 */
void main()
{
    vec3 pos = aPos + aNormal * outline;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
