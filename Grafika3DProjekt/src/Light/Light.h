#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Rendering/Shader.h"

/**
 * @class Light
 * @brief Base class for all light types.
 *
 * Stores:
 * - Light color
 * - Ambient intensity
 * - Diffuse intensity
 *
 * Used as a parent for DirectionalLight, PointLight, Flashlight, etc.
 */
class Light
{
public:

    /**
     * @brief Creates a basic light.
     *
     * @param colors Light color.
     * @param ambientIntensity Ambient light strength.
     * @param diffuseIntensity Diffuse light strength.
     */
    Light(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity);

    /** @brief Destructor. */
    ~Light();

    /** @brief Sets ambient intensity. */
    void setAmbientIntensity(GLfloat intensity) { lightAmbientIntensity = intensity; }

    /** @brief Sets diffuse intensity. */
    void setDiffuseIntensity(GLfloat intensity) { lightDiffuseIntensity = intensity; }

    /** @brief Gets ambient intensity. */
    GLfloat getAmbientIntensity() const { return lightAmbientIntensity; }

    /** @brief Gets diffuse intensity. */
    GLfloat getDiffuseIntensity() const { return lightDiffuseIntensity; }

protected:
    glm::vec3 lightColor;          ///< Light color.
    GLfloat lightAmbientIntensity; ///< Ambient strength.
    GLfloat lightDiffuseIntensity; ///< Diffuse strength.
};
