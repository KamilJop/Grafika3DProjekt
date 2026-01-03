#include "Flashlight.h"

/**
 * @brief Creates a flashlight (spotlight) with shadow support.
 *
 * @param colors Light color.
 * @param ambientIntensity Ambient strength.
 * @param diffuseIntensity Diffuse strength.
 * @param lightPos Light position.
 * @param con Constant attenuation.
 * @param lin Linear attenuation.
 * @param quad Quadratic attenuation.
 * @param direction Direction the flashlight points.
 * @param cutOff Inner cutoff angle (degrees).
 * @param outerCutOff Outer cutoff angle (degrees).
 * @param shadowWidth Shadow map width.
 * @param shadowHeight Shadow map height.
 */
Flashlight::Flashlight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity,
                       glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad,
                       glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff,
                       GLfloat shadowWidth, GLfloat shadowHeight)
    : PointLight(colors, ambientIntensity, diffuseIntensity, lightPos, con, lin, quad)
{
    lightDirection = direction;

    lightCutOff = glm::cos(glm::radians(cutOff));
    lightOuterCutOff = glm::cos(glm::radians(outerCutOff));

    cutoffDegrees = cutOff;
    outerCutoffDegrees = outerCutOff;

    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);

    float aspect = shadowWidth / shadowHeight;
    float fov = outerCutoffDegrees * 2.0f;

    lightProjection = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);
}

/**
 * @brief Destructor resets values and frees shadow map.
 */
Flashlight::~Flashlight()
{
    lightColor = glm::vec3(0.0f);
    lightPosition = glm::vec3(0.0f);
    lightDirection = glm::vec3(0.0f);

    lightAmbientIntensity = 0.0f;
    lightDiffuseIntensity = 0.0f;

    lightCutOff = 0.0f;
    lightOuterCutOff = 0.0f;

    constant = linear = quadratic = 0.0f;

    if (shadowMap)
    {
        delete shadowMap;
        shadowMap = nullptr;
    }
}

/**
 * @brief Sets the flashlight position.
 */
void Flashlight::setLightPosition(glm::vec3 pos)
{
    lightPosition = pos;
}

/**
 * @brief Sets the flashlight direction.
 */
void Flashlight::setLightDirection(glm::vec3 dir)
{
    lightDirection = dir;
}

/**
 * @brief Sends flashlight data to the shader.
 *
 * @param lightShader Shader receiving the uniforms.
 */
void Flashlight::useLight(Shader* lightShader)
{
    lightShader->setVec3("flashLight.lightColor", lightColor);
    lightShader->setFloat("flashLight.ambientIntensity", lightAmbientIntensity);
    lightShader->setFloat("flashLight.diffuseIntensity", lightDiffuseIntensity);

    lightShader->setVec3("flashLight.lightPosition", lightPosition);
    lightShader->setVec3("flashLight.lightDirection", lightDirection);

    lightShader->setFloat("flashLight.cutOff", lightCutOff);
    lightShader->setFloat("flashLight.outerCutOff", lightOuterCutOff);

    lightShader->setFloat("flashLight.constant", constant);
    lightShader->setFloat("flashLight.linear", linear);
    lightShader->setFloat("flashLight.quadratic", quadratic);
}

/**
 * @brief Computes the flashlight's projection * view matrix.
 *
 * Used for shadow mapping.
 *
 * @return Light space transform matrix.
 */
glm::mat4 Flashlight::CalculateLightTransform()
{
    glm::mat4 lightView = glm::lookAt(
        lightPosition,
        lightPosition + lightDirection,
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    return lightProjection * lightView;
}
