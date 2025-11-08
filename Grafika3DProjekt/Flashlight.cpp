#include "Flashlight.h"

// Constructor
Flashlight::Flashlight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff)
	: PointLight(colors, ambientIntensity, diffuseIntensity, lightPos, con, lin, quad)
{
	lightDirection = direction;
	lightCutOff = cutOff;
	lightOuterCutOff = outerCutOff;
}

// Destructor
Flashlight::~Flashlight()
{	
	lightColor = glm::vec3(0.0f);
	lightPosition = glm::vec3(0.0f);
	lightDirection = glm::vec3(0.0f);
	lightAmbientIntensity = 0.0f;
	lightDiffuseIntensity = 0.0f;
	lightCutOff = 0.0f;
	lightOuterCutOff = 0.0f;
	constant = 0.0f;
	linear = 0.0f;
	quadratic = 0.0f;
}

// Method to set light position
void Flashlight::setLightPosition(glm::vec3 pos)
{
	lightPosition = pos;
}	

// Method to set light direction
void Flashlight::setLightDirection(glm::vec3 dir)
{
	lightDirection = dir;
}

// Method used to set light uniforms in shader
void Flashlight::useLight(Shader* lightShader)
{
	lightShader->setVec3("flashLight.lightColor", lightColor);
	lightShader->setFloat("flashLight.ambientIntensity", lightAmbientIntensity);
	lightShader->setFloat("flashLight.diffuseIntensity", lightDiffuseIntensity);
	lightShader->setVec3("flashLight.lightPosition", lightPosition);
	lightShader->setVec3("flashLight.lightDirection", lightDirection);
	lightShader->setFloat("flashLight.cutOff", glm::cos(glm::radians(lightCutOff)));
	lightShader->setFloat("flashLight.outerCutOff", glm::cos(glm::radians(lightOuterCutOff)));
	lightShader->setFloat("flashLight.constant", constant);
	lightShader->setFloat("flashLight.linear", linear);
	lightShader->setFloat("flashLight.quadratic", quadratic);
}