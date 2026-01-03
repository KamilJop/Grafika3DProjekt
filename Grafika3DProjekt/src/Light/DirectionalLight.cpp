#include "DirectionalLight.h"

/**
 * @brief Creates a directional light with optional shadow mapping.
 *
 * @param colors Light color.
 * @param dir Direction the light shines toward.
 * @param ambientIntensity Ambient light strength.
 * @param diffuseIntensity Diffuse light strength.
 * @param shadowWidth Shadow map width.
 * @param shadowHeight Shadow map height.
 */
DirectionalLight::DirectionalLight(glm::vec3 colors, glm::vec3 dir,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat shadowWidth, GLfloat shadowHeight)
    : Light(colors, ambientIntensity, diffuseIntensity)
{
    lightDirection = dir;

    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);

    // Orthographic projection for directional light shadows
    lightProjection = glm::ortho(-20.0f, 20.0f,
                                 -20.0f, 20.0f,
                                  1.0f, 100.0f);
}

/**
 * @brief Destructor resets light values.
 */
DirectionalLight::~DirectionalLight()
{
    lightDirection = glm::vec3(0.0f);
    lightColor = glm::vec3(0.0f);
    lightAmbientIntensity = 0.0f;
    lightDiffuseIntensity = 0.0f;
}

/**
 * @brief Sends light data to the shader.
 *
 * @param lightShader Shader that receives the light uniforms.
 */
void DirectionalLight::useLight(Shader* lightShader)
{
    lightShader->setVec3("directionalLight.lightColor", lightColor);
    lightShader->setVec3("directionalLight.lightDirection", lightDirection);
    lightShader->setFloat("directionalLight.lightAmbientIntensity", lightAmbientIntensity);
    lightShader->setFloat("directionalLight.lightDiffuseIntensity", lightDiffuseIntensity);
}

/**
 * @brief Computes the light's projection * view matrix.
 *
 * Used for shadow mapping.
 *
 * @return Light space transform matrix.
 */
glm::mat4 DirectionalLight::CalculateLightTransform()
{
    glm::mat4 lightView = glm::lookAt(
        -lightDirection,          // Light position
        glm::vec3(0.0f),          // Look at origin
        glm::vec3(0.0f, 1.0f, 0.0f) // Up vector
    );

    return lightProjection * lightView;
}
