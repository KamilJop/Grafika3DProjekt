/**
 * @file directional_shadow_map.vert
 * @ingroup Shaders
 * @brief Depth vertex shader for directional light shadow mapping
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
