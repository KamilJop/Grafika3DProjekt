#include "PointLight.h"
#include <string>

// Constructor
PointLight::PointLight(glm::vec3 colors = glm::vec3(1.0f), GLfloat ambientIntensity = 0.1f, GLfloat diffuseIntensity = 0.1f, glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 0.0f), GLfloat con = 1.0f, GLfloat lin = 0.09f, GLfloat quad = 0.032f, int index = 0, GLfloat far = 100.0f, GLfloat near = 0.01f, GLfloat shadowH = 2048.0f, GLfloat shadowW = 2048.0f)
	: Light(colors, ambientIntensity, diffuseIntensity)
{
	constant = con;
	linear = lin;
	quadratic = quad;
	lightPosition = lightPos;
	lightIndex = index;
	farPlane = far;
	nearPlane = near;
	shadowHeight = shadowH;
	shadowWidth = shadowW;
	shadowMap = new OmniShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
	float aspect = shadowWidth / shadowHeight;
	lightProjection = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

}

// Constructor used for flashlight inheritance
PointLight::PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, int index)
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

	std::string shadowBase = "omniShadowMaps[" + indexString + "].";

	lightShader->setFloat(shadowBase + "farPlane", farPlane);
	int textureUnit = 3 + lightIndex;
	shadowMap->Read(GL_TEXTURE0 + textureUnit);
	lightShader->setInt(shadowBase + "shadowMap", textureUnit);
}

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
	std::vector<glm::mat4> lightTransforms;
	// + X
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	// - X
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	// + Y
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	// - Y
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	// + Z
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	// - Z
	lightTransforms.push_back(lightProjection * glm::lookAt(lightPosition, lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	return lightTransforms;
}