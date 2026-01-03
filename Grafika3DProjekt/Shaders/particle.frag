/**
 * @file particle.frag
 * @brief Fragment shader for rendering particles with bloom
 */

#version 330 core

/** Output color */
layout (location = 0) out vec4 FragColor;

/** Output color for bright areas (used for bloom) */
layout (location = 1) out vec4 BrightColor;

/** Texture coordinates from vertex shader */
in vec2 TexCoords;

/** Particle color */
in vec4 ParticleColor;

/** Particle sprite texture */
uniform sampler2D sprite;

/**
 * @brief Main shader function
 *
 * Applies particle color and texture. Discards transparent fragments.
 * Outputs both normal color and bright color for bloom effect.
 */
void main()
{
    vec4 texColor = texture(sprite, TexCoords) * ParticleColor;

    if (texColor.a < 0.1) discard;

    FragColor = texColor;
    BrightColor = texColor; 
}
