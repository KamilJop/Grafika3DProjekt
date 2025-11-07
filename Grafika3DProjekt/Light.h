#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
class Light
{
public:
	Light(glm::vec3 colors, glm::vec3 dir, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~Light();
	void useLight(Shader* lightShader);

private:
	glm::vec3 lightColor;
	glm::vec3 lightDirection;
	GLfloat lightAmbientIntensity;
	GLfloat lightDiffuseIntensity;
};

