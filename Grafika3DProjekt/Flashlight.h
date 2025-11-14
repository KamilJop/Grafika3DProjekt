#pragma once
#include "PointLight.h"
#include <glm/glm.hpp>
#include "ShadowMap.h"

class Flashlight : PointLight
{
public:
	Flashlight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, glm::vec3 direction, GLfloat cutOff, GLfloat outerCutOff, GLfloat shadowWidth, GLfloat shadowHeight);
	~Flashlight();
	void setLightPosition(glm::vec3 pos);
	void setLightDirection(glm::vec3 dir);
	void useLight(Shader* lightShader);
	ShadowMap* getShadowMap() { return shadowMap; }
	glm::mat4 CalculateLightTransform();
	

private:
	glm::vec3 lightDirection;
	GLfloat lightCutOff;
	GLfloat lightOuterCutOff;
	ShadowMap* shadowMap;
	glm::mat4 lightProjection;
};

