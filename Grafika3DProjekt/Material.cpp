#include "Material.h"

// Constructor
Material::Material(GLfloat intensity, GLfloat shine)
{
	specularIntensity = intensity;
	shininess = shine;
}
// Destructor
Material::~Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

// Method used to set material uniforms in shader
void Material::useMaterial(Shader* shader)
{
	shader->setFloat("material.specularIntensity", specularIntensity);
	shader->setFloat("material.shininess", shininess);
}