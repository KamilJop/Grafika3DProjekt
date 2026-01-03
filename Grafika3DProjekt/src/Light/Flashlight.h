#pragma once
#include "PointLight.h"
#include <glm/glm.hpp>
#include "ShadowMap.h"

/**
 * @class Flashlight
 * @brief A spotlight-style light used as a player flashlight.
 *
 * Extends PointLight with:
 * - Direction
 * - Inner and outer cutoff angles
 * - Shadow mapping support
 */
class Flashlight : PointLight
{
public:

    /**
     * @brief Creates a flashlight with direction, cutoff angles, and shadow map.
     */
    Flashlight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity,
               glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad,
               glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff,
               GLfloat shadowWidth, GLfloat shadowHeight);

    /** @brief Destructor. */
    ~Flashlight();

    /** @brief Sets the flashlight position. */
    void setLightPosition(glm::vec3 pos);

    /** @brief Sets the flashlight direction. */
    void setLightDirection(glm::vec3 dir);

    /** @brief Sends flashlight data to the shader. */
    void useLight(Shader* lightShader);

    /** @brief Returns the shadow map used by the flashlight. */
    ShadowMap* getShadowMap() { return shadowMap; }

    /** @brief Computes the light's projection * view matrix. */
    glm::mat4 CalculateLightTransform();

private:
    glm::vec3 lightDirection;     ///< Direction the flashlight points.
    GLfloat lightCutOff;          ///< Inner cutoff (cosine).
    GLfloat lightOuterCutOff;     ///< Outer cutoff (cosine).
    GLfloat cutoffDegrees;        ///< Inner cutoff in degrees.
    GLfloat outerCutoffDegrees;   ///< Outer cutoff in degrees.
    ShadowMap* shadowMap;         ///< Shadow map for the flashlight.
    glm::mat4 lightProjection;    ///< Projection matrix for shadow mapping.
};
