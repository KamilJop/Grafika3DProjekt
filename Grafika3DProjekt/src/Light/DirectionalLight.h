#pragma once
#include "Light.h"
#include "ShadowMap.h"

/**
 * @class DirectionalLight
 * @brief A light source that shines in a single direction, used for sun/moon lighting.
 *
 * Supports:
 * - Ambient and diffuse intensity
 * - Shadow mapping
 * - Light‑space transform calculation
 */
class DirectionalLight : Light
{
public:

    /**
     * @brief Creates a directional light.
     *
     * @param colors Light color.
     * @param dir Direction the light shines toward.
     * @param ambientIntensity Ambient light strength.
     * @param diffuseIntensity Diffuse light strength.
     * @param shadowWidth Shadow map width.
     * @param shadowHeight Shadow map height.
     */
    DirectionalLight(glm::vec3 colors, glm::vec3 dir,
                     GLfloat ambientIntensity, GLfloat diffuseIntensity,
                     GLfloat shadowWidth, GLfloat shadowHeight);

    /** @brief Destructor. */
    ~DirectionalLight();

    /**
     * @brief Sends light data to the shader.
     * @param lightShader Shader receiving the light uniforms.
     */
    void useLight(Shader* lightShader);

    /** @brief Returns the shadow map used by this light. */
    ShadowMap* getShadowMap() { return shadowMap; }

    /** @brief Sets ambient intensity. */
    void setAmbientIntensity(GLfloat intensity) { lightAmbientIntensity = intensity; }

    /** @brief Sets diffuse intensity. */
    void setDiffuseIntensity(GLfloat intensity) { lightDiffuseIntensity = intensity; }

    /** @brief Gets ambient intensity. */
    float getAmbientIntensity() { return lightAmbientIntensity; }

    /** @brief Gets diffuse intensity. */
    float getDiffuseIntensity() { return lightDiffuseIntensity; }

    /**
     * @brief Computes the light's projection * view matrix (used for shadows).
     * @return Light space transform matrix.
     */
    glm::mat4 CalculateLightTransform();

private:
    glm::vec3 lightDirection;   ///< Direction the light shines toward.
    ShadowMap* shadowMap;       ///< Shadow map used for depth rendering.
    glm::mat4 lightProjection;  ///< Orthographic projection for shadow mapping.
};
