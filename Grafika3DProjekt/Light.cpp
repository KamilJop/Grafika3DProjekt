#include "Light.h"


// Constructor
Light::Light(glm::vec3 colors = glm::vec3(1.0f), glm::vec3 dir = glm::vec3(0.0f,0.1f,0.0f), GLfloat ambientIntensity = 0.1f, GLfloat diffuseIntensity = 0.1f)
{
	lightColor = colors;
	lightDirection = dir;
	lightAmbientIntensity = ambientIntensity;
	lightDiffuseIntensity = diffuseIntensity;
}

// Destructor
Light::~Light()
{
	lightColor = glm::vec3(0.0f);
	lightDirection = glm::vec3(0.0f);
	lightAmbientIntensity = 0.0f;
}

// Method used to set light uniforms in shader
void Light::useLight(Shader* lightShader)
{
	lightShader->setVec3("light.lightColor", lightColor);
	lightShader->setVec3("light.lightDirection", lightDirection);
	lightShader->setFloat("light.lightAmbientIntensity", lightAmbientIntensity);
	lightShader->setFloat("light.lightDiffuseIntensity", lightDiffuseIntensity);
}