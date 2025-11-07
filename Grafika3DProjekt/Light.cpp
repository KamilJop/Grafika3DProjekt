#include "Light.h"


// Constructor
Light::Light(glm::vec3 colors = glm::vec3(1.0f), glm::vec3 pos = glm::vec3(0.0f), GLfloat ambientIntensity = 0.1f)
{
	lightColor = colors;
	lightPos = pos;
	lightAmbientIntensity = ambientIntensity;
}

// Destructor
Light::~Light()
{
	lightColor = glm::vec3(0.0f);
	lightPos = glm::vec3(0.0f);
	lightAmbientIntensity = 0.0f;
}

// Method used to set light uniforms in shader
void Light::useLight(Shader* lightShader)
{
	lightShader->setVec3("light.lightColor", lightColor);
	lightShader->setVec3("light.lightPos", lightPos);
	lightShader->setFloat("light.lightAmbientIntensity", lightAmbientIntensity);
}