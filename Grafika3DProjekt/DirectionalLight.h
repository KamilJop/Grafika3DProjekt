#pragma once
#include "Light.h"
class DirectionalLight : Light
{
	public:
	DirectionalLight(glm::vec3 colors, glm::vec3 dir, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~DirectionalLight();
	void useLight(Shader* lightShader);
private:
	glm::vec3 lightDirection;
};

