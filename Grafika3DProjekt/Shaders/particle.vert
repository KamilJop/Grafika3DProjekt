/**
 * @file particle.vert
 * @brief Vertex shader for rendering particles
 */

#version 330 core

/** Vertex position */
layout (location = 0) in vec3 aPos;

/** Texture coordinates */
layout (location = 1) in vec2 aTexCoords;

/** Passed texture coordinates */
out vec2 TexCoords;

/** Passed particle color */
out vec4 ParticleColor;

/** Projection matrix */
uniform mat4 projection;

/** View matrix */
uniform mat4 view;

/** Particle offset in world space */
uniform vec3 offset;

/** Particle color */
uniform vec4 color;

/** Particle scale */
uniform float scale;

/**
 * @brief Main shader function
 *
 * Computes the world position of a particle quad and passes
 * texture coordinates and color to the fragment shader.
 */
void main()
{
    TexCoords = aTexCoords;
    ParticleColor = color;

    vec3 CameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 CameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexPosition_world = offset 
                              + CameraRight * aPos.x * scale 
                              + CameraUp * aPos.y * scale;

    gl_Position = projection * view * vec4(vertexPosition_world, 1.0);
}
