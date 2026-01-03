/**
 * @file hdr.frag
 * @brief HDR tone mapping fragment shader
 */

#version 330 core

/** Fragment output color */
out vec4 FragColor;

/** Texture coordinates */
in vec2 TexCoords;

/** HDR color buffer */
uniform sampler2D hdrBuffer;

/** Bloom blur texture */
uniform sampler2D bloomBlur;

/** Exposure value for tone mapping */
uniform float exposure;

/**
 * @brief Main shader function
 */
void main()
{
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    // Add bloom effect
    hdrColor += bloomColor;

    // Apply exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    FragColor = vec4(mapped, 1.0);
}
