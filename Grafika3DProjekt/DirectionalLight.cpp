#include "DirectionalLight.h"

// Constructor
DirectionalLight::DirectionalLight(glm::vec3 colors = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat ambientIntensity = 0.1f, GLfloat diffuseIntensity = 0.1f)
	: Light(colors, ambientIntensity, diffuseIntensity)
{
	lightDirection = dir;
}

// Destructor
DirectionalLight::~DirectionalLight()
{
	lightDirection = glm::vec3(0.0f);
	lightColor = glm::vec3(0.0f);
	lightAmbientIntensity = 0.0f;
	lightDiffuseIntensity = 0.0f;
}

// Method used to set light uniforms in shader
void DirectionalLight::useLight(Shader* lightShader)
{
	lightShader->setVec3("directionalLight.lightColor", lightColor);
	lightShader->setVec3("directionalLight.lightDirection", lightDirection);
	lightShader->setFloat("directionalLight.lightAmbientIntensity", lightAmbientIntensity);
	lightShader->setFloat("directionalLight.lightDiffuseIntensity", lightDiffuseIntensity);
}