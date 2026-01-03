/**
 * @file lighting.vert
 * @brief Vertex shader for transforming geometry and preparing data for lighting.
 *
 * Outputs:
 * - World‑space position and normal
 * - Texture coordinates
 * - Shadow map positions (directional + flashlight)
 * - TBN matrix for normal/parallax mapping
 * - Tangent‑space view and fragment positions
 */

#version 330

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

// Outputs to fragment shader
out vec4 vCol;
out vec3 Normal;
out vec3 FragPos;
out vec2 TextureCoordinates;
out vec4 DirectionalLightSpacePosition;
out vec4 FlashLightSpacePosition;
out mat3 TBN;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

// Uniforms
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightSpaceTransform;
uniform mat4 flashLightSpaceTransform;
uniform vec3 cameraPosition;

/**
 * @brief Main vertex shader entry point.
 */
void main()
{
    // Final clip‑space position
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    // Shadow map positions
    DirectionalLightSpacePosition = directionalLightSpaceTransform * model * vec4(aPos, 1.0);
    FlashLightSpacePosition       = flashLightSpaceTransform       * model * vec4(aPos, 1.0);

    // World‑space fragment position and normal
    FragPos = (model * vec4(aPos, 1.0)).xyz;
    Normal  = mat3(transpose(inverse(model))) * aNormal;

    // Texture coordinates
    TextureCoordinates = aTexture;
    vCol = vec4(1.0);

    // ----------------------------
    // Build TBN matrix for normal mapping
    // ----------------------------
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

    // Orthogonalize tangent
    T = normalize(T - dot(T, N) * N);

    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    // ----------------------------
    // Convert positions to tangent space (for parallax mapping)
    // ----------------------------
    mat3 TBN_inverse = transpose(TBN);

    TangentViewPos = TBN_inverse * cameraPosition;
    TangentFragPos = TBN_inverse * FragPos;
}
