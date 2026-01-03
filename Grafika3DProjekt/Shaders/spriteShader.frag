/**
 * @file sprite.frag
 * @brief Fragment shader for rendering 2D sprites.
 *
 * Multiplies the sprite texture by a tint color.
 */

#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    // Tint the sprite and sample the texture
    color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
}
