/**
 * @file textShader.frag
 * @ingroup Shaders
 * @brief Fragment shader for rendering bitmap text.
 *
 * Samples the glyph texture (stored in the red channel)
 * and multiplies it by the desired text color.
 */

#version 330

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;     // Glyph atlas (R channel = alpha)
uniform vec4 textColor;     // Final text color

void main()
{
    // Sample glyph alpha from red channel
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);

    // Apply text color
    color = textColor * sampled;
}
