#include "Light.h"

/**
 * @brief Base light class constructor.
 *
 * Initializes the light color and its ambient/diffuse intensity values.
 *
 * @param colors Light color.
 * @param ambientIntensity Ambient light strength.
 * @param diffuseIntensity Diffuse light strength.
 */
Light::Light(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity)
{
    lightColor = colors;
    lightAmbientIntensity = ambientIntensity;
    lightDiffuseIntensity = diffuseIntensity;
}

/**
 * @brief Destructor resets light values.
 */
Light::~Light()
{
    lightColor = glm::vec3(0.0f);
    lightAmbientIntensity = 0.0f;
    lightDiffuseIntensity = 0.0f;
}
