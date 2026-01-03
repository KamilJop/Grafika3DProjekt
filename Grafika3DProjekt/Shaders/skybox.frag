/**
 * @file skybox.frag
 * @ingroup Shaders
 * @brief Fragment shader for rendering the skybox.
 *
 * Samples a cubemap texture using the incoming direction vector.
 */

#version 330

in vec3 TexCoords;
out vec4 colour;

uniform samplerCube skybox;

void main()
{
    // Sample cubemap
    colour = texture(skybox, TexCoords);
}
