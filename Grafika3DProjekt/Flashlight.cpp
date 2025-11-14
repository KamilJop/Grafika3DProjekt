#include "Flashlight.h"

// Constructor
Flashlight::Flashlight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff, GLfloat shadowWidth, GLfloat shadowHeight)
	: PointLight(colors, ambientIntensity, diffuseIntensity, lightPos, con, lin, quad)
{
	lightDirection = direction;
	lightCutOff = cutOff;
	// Potem do wyrzucenia :D
	lightPos = lightPosition = glm::vec3(0.0f, 1.0f, 2.0f);
	lightOuterCutOff = outerCutOff;
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
	float aspect = shadowWidth / shadowHeight;
	lightProjection = glm::perspective(glm::radians(outerCutOff * 2.0f), aspect, 0.1f, 100.0f);
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
	if (shadowMap)
	{
		delete shadowMap;
		shadowMap = nullptr;
	}
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

glm::mat4 Flashlight::CalculateLightTransform()
{
	// Calculate view matrix for light
	glm::mat4 lightView = glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));

	// Calculate light projection-view matrix
	return lightProjection * lightView;
}