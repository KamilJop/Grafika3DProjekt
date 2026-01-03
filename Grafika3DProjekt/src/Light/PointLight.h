#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>

/**
 * @class PointLight
 * @brief A light that emits in all directions from a single point.
 *
 * Supports:
 * - Attenuation (constant, linear, quadratic)
 * - Cube‑map shadow mapping (OmniShadowMap)
 * - Multiple point lights via index
 */
class PointLight : public Light
{
public:

    /**
     * @brief Creates a point light with shadow mapping enabled.
     *
     * @param colors Light color.
     * @param ambientIntensity Ambient strength.
     * @param diffuseIntensit Diffuse strength.
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
    PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensit,
               glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad,
               int index, GLfloat far, GLfloat near,
               GLfloat shadowH, GLfloat shadowW);

    /**
     * @brief Constructor used when inherited by Flashlight (no shadow setup).
     */
    PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensit,
               glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad,
               int index = 0);

    /** @brief Destructor. */
    ~PointLight();

    /** @brief Sends point light data to the shader. */
    void useLight(Shader* lightShader);

    /** @brief Sets the light position. */
    void setPosition(glm::vec3 pos) { lightPosition = pos; }

    /** @brief Sets the light color. */
    void setColor(glm::vec3 color) { lightColor = color; }

    /** @brief Enables or disables shadow casting. */
    void setCastShadow(bool cast) { castShadow = cast; }

    /** @brief Returns whether the light casts shadows. */
    bool getCastShadow() { return castShadow; }

    /** @brief Returns the light color. */
    glm::vec3 getColor() { return lightColor; }

    /** @brief Returns the light position. */
    glm::vec3 getPosition() { return lightPosition; }

    /** @brief Returns the shadow map used by this point light. */
    OmniShadowMap* getShadowMap() { return shadowMap; }

    /** @brief Returns the light position (duplicate accessor). */
    glm::vec3 getLightPosition() { return lightPosition; }

    /** @brief Returns the far plane distance used for shadows. */
    GLfloat getFarPlane() { return farPlane; }

    /**
     * @brief Computes the 6 projection-view matrices for cube‑map shadow rendering.
     *
     * @return A vector of 6 matrices.
     */
    std::vector<glm::mat4> calculateLightTransform();

protected:
    glm::vec3 lightPosition;   ///< Light world position.
    GLfloat constant;          ///< Constant attenuation.
    GLfloat linear;            ///< Linear attenuation.
    GLfloat quadratic;         ///< Quadratic attenuation.

    GLfloat farPlane;          ///< Far plane for shadow projection.
    GLfloat nearPlane;         ///< Near plane for shadow projection.

    GLfloat shadowHeight;      ///< Shadow map height.
    GLfloat shadowWidth;       ///< Shadow map width.

    OmniShadowMap* shadowMap;  ///< Cube‑map shadow map.

    glm::mat4 lightProjection; ///< Projection matrix for shadow mapping.

    int lightIndex;            ///< Index in shader array.
    bool castShadow = false;   ///< Whether this light casts shadows.
};
