#include "PointLight.h"
#include <string>

// Constructor
PointLight::PointLight(glm::vec3 colors = glm::vec3(1.0f), GLfloat ambientIntensity = 0.1f, GLfloat diffuseIntensity = 0.1f, glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 0.0f), GLfloat con = 1.0f, GLfloat lin = 0.09f, GLfloat quad = 0.032f, int index)
	: Light(colors, ambientIntensity, diffuseIntensity)
{
	constant = con;
	linear = lin;
	quadratic = quad;
	lightPosition = lightPos;
	lightIndex = index;
}

// Destructor
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

// Method used to set light uniforms in shader
void PointLight::useLight(Shader* lightShader)
{
	std::string indexString = std::to_string(lightIndex);
	std::string base = "pointLights[" + indexString + "].";

	lightShader->setVec3(base + "lightColor", lightColor);
	lightShader->setVec3(base + "lightPosition", lightPosition);
	lightShader->setFloat(base + "lightAmbientIntensity", lightAmbientIntensity);
	lightShader->setFloat(base + "lightDiffuseIntensity", lightDiffuseIntensity);
	lightShader->setFloat(base + "constant", constant);
	lightShader->setFloat(base + "linear", linear);
	lightShader->setFloat(base + "quadratic", quadratic);
}