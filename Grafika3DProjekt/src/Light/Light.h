#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Rendering/Shader.h"
class Light
{
public:
	Light(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~Light();

protected:
	glm::vec3 lightColor;
	GLfloat lightAmbientIntensity;
	GLfloat lightDiffuseIntensity;
};

