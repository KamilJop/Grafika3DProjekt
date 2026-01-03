/**
 * @file omni_shadow_map.vert
 * @ingroup Shaders
 * @brief Vertex shader for point light shadow mapping
 */

#version 330

/** Vertex position */
layout (location = 0) in vec3 pos;

/** Model transformation matrix */
uniform mat4 model;

/**
 * @brief Main shader function
 */
void main()
{
    gl_Position = model * vec4(pos, 1.0);
}
