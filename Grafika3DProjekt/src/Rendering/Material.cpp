#include "Material.h"

/**
 * @brief Creates a material with specular intensity and shininess.
 *
 * @param intensity Strength of the specular highlight.
 * @param shine Shininess factor controlling highlight size.
 */
Material::Material(GLfloat intensity, GLfloat shine)
{
    specularIntensity = intensity;
    shininess = shine;
}

/**
 * @brief Destructor resets material values.
 */
Material::~Material()
{
    specularIntensity = 0.0f;
    shininess = 0.0f;
}

/**
 * @brief Sends material properties to the shader.
 *
 * Sets:
 * - material.specularIntensity
 * - material.shininess
 *
 * @param shader Shader receiving the material uniforms.
 */
void Material::useMaterial(Shader* shader)
{
    shader->setFloat("material.specularIntensity", specularIntensity);
    shader->setFloat("material.shininess", shininess);
}
