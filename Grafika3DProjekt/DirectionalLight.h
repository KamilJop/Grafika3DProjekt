#pragma once
#include "Light.h"
#include "ShadowMap.h"
class DirectionalLight : Light
{
	public:
	DirectionalLight(glm::vec3 colors, glm::vec3 dir, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat shadowWidth, GLfloat shadowHeight);
	~DirectionalLight();
	void useLight(Shader* lightShader);
	ShadowMap* getShadowMap() { return shadowMap; }
	glm::mat4 CalculateLightTransform();
private:
	glm::vec3 lightDirection;
	ShadowMap* shadowMap;
	glm::mat4 lightProjection;
};

