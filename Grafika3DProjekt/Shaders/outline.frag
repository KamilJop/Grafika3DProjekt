/**
 * @file outline.frag
 * @ingroup Shaders
 * @brief Fragment shader for rendering an outline color
 */

#version 330 core

/** Output fragment color */
out vec4 FragColor;

/** Color used for the outline */
uniform vec3 outlineColor;

/**
 * @brief Main shader function
 */
void main()
{
    FragColor = vec4(outlineColor, 1.0);
}
