/**
 * @file depth.frag
 * @brief Depth fragment shader for point light shadow mapping
 */

#version 330

/** Fragment position in world space */
in vec4 FragPos;

/** Light position */
uniform vec3 lightPos;

/** Far plane distance */
uniform float farPlane;

/**
 * @brief Main shader function
 */
void main()
{
    float distance = length(FragPos.xyz - lightPos);
    distance = distance / farPlane;
    gl_FragDepth = distance;
}
