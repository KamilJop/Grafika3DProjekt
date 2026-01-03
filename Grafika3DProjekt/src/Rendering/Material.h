#pragma once

#include <glad/glad.h>
#include "Shader.h"

/**
 * @class Material
 * @brief Stores specular properties used in lighting calculations.
 *
 * Provides:
 * - Specular intensity (strength of highlight)
 * - Shininess (size of highlight)
 *
 * Used to configure material-related uniforms in shaders.
 */
class Material
{
public:

    /**
     * @brief Creates a material with specular intensity and shininess.
     *
     * @param intensity Strength of the specular highlight.
     * @param shine Shininess factor controlling highlight size.
     */
    Material(GLfloat intensity, GLfloat shine);

    /** @brief Destructor. */
    ~Material();

    /**
     * @brief Sends material properties to the shader.
     *
     * Sets:
     * - material.specularIntensity
     * - material.shininess
     *
     * @param shader Shader receiving the material uniforms.
     */
    void useMaterial(Shader* shader);

private:
    GLfloat specularIntensity; ///< Strength of specular reflection.
    GLfloat shininess;         ///< Controls the sharpness of highlights.
};
