/**
 * @file depth.vert
 * @brief Depth vertex shader
 */

#version 330

/** Vertex position */
layout (location = 0) in vec3 pos;

/** Model matrix */
uniform mat4 model;

/** Light space transformation matrix */
uniform mat4 lightSpaceTransform;

/**
 * @brief Main shader function
 */
void main()
{
    gl_Position = lightSpaceTransform * model * vec4(pos, 1.0);
}
