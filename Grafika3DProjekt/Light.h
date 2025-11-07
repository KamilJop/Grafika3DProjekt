#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
class Light
{
public:
	Light(glm::vec3 colors, glm::vec3 pos, GLfloat ambientIntensity);
	~Light();
	void useLight(Shader* lightShader);

private:
	glm::vec3 lightColor;
	glm::vec3 lightPos;
	GLfloat lightAmbientIntensity;
};

