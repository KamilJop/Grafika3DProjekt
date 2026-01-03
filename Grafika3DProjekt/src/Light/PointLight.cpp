#include "PointLight.h"
#include <string>

/**
 * @brief Creates a point light with shadow mapping support.
 *
 * Includes:
 * - Light color and intensity
 * - Position and attenuation factors
 * - Cube‑map shadow map (OmniShadowMap)
 *
 * @param colors Light color.
 * @param ambientIntensity Ambient strength.
 * @param diffuseIntensity Diffuse strength.
 * @param lightPos Light position.
 * @param con Constant attenuation.
 * @param lin Linear attenuation.
 * @param quad Quadratic attenuation.
 * @param index Index in shader array.
 * @param far Far plane for shadow projection.
 * @param near Near plane for shadow projection.
 * @param shadowH Shadow map height.
 * @param shadowW Shadow map width.
 */
PointLight::PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity,
                       glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad,
                       int index, GLfloat far, GLfloat near,
                       GLfloat shadowH, GLfloat shadowW)
    : Light(colors, ambientIntensity, diffuseIntensity)
{
    constant = con;
    linear = lin;
    quadratic = quad;

    lightPosition = lightPos;
    lightIndex = index;

    farPlane = far;
    nearPlane = near;

    shadowHeight = shadowH;
    shadowWidth = shadowW;

    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);

    float aspect = shadowWidth / shadowHeight;
    lightProjection = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);
}

/**
 * @brief Constructor used when inherited by Flashlight (no shadow setup).
 */
PointLight::PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity,
                       glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, int index)
    : Light(colors, ambientIntensity, diffuseIntensity)
{
    constant = con;
    linear = lin;
    quadratic = quad;

    lightPosition = lightPos;
    lightIndex = index;
}

/**
 * @brief Destructor resets values.
 */
PointLight::~PointLight()
{
    constant = 0.0f;
    linear = 0.0f;
    quadratic = 0.0f;

    lightColor = glm::vec3(0.0f);
    lightPosition = glm::vec3(0.0f);

    lightAmbientIntensity = 0.0f;
    lightDiffuseIntensity = 0.0f;
}

/**
 * @brief Sends point light data to the shader.
 *
 * Includes:
 * - Color, position, intensities
 * - Attenuation factors
 * - Shadow map binding
 *
 * @param lightShader Shader receiving the uniforms.
 */
void PointLight::useLight(Shader* lightShader)
{
    std::string indexString = std::to_string(lightIndex);
    std::string base = "pointLights[" + indexString + "].";

    lightShader->setInt(base + "castsShadows", castShadow ? 1 : 0);
    lightShader->setVec3(base + "lightColor", lightColor);
    lightShader->setVec3(base + "lightPosition", lightPosition);
    lightShader->setFloat(base + "lightAmbientIntensity", lightAmbientIntensity);
    lightShader->setFloat(base + "lightDiffuseIntensity", lightDiffuseIntensity);
    lightShader->setFloat(base + "constant", constant);
    lightShader->setFloat(base + "linear", linear);
    lightShader->setFloat(base + "quadratic", quadratic);

    std::string shadowBase = "omniShadowMaps[" + indexString + "].";

    lightShader->setFloat(shadowBase + "farPlane", farPlane);

    int textureUnit = 5 + lightIndex;
    shadowMap->Read(GL_TEXTURE0 + textureUnit);
    lightShader->setInt(shadowBase + "shadowMap", textureUnit);
}

/**
 * @brief Computes the 6 projection-view matrices for cube‑map shadow rendering.
 *
 * @return A vector of 6 matrices (one for each cube face).
 */
std::vector<glm::mat4> PointLight::calculateLightTransform()
{
    std::vector<glm::mat4> lightTransforms;

    // +X
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));

    // -X
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));

    // +Y
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));

    // -Y
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));

    // +Z
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));

    // -Z
    lightTransforms.push_back(lightProjection *
        glm::lookAt(lightPosition, lightPosition + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));

    return lightTransforms;
}
