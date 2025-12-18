#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Rendering/Shader.h"
class Light
{
public:
	Light(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensity);
	~Light();
	void setAmbientIntensity(GLfloat intensity) { lightAmbientIntensity = intensity; }
	void setDiffuseIntensity(GLfloat intensity) { lightDiffuseIntensity = intensity; }
	GLfloat getAmbientIntensity() const { return lightAmbientIntensity; }
	GLfloat getDiffuseIntensity() const { return lightDiffuseIntensity; }
protected:
	glm::vec3 lightColor;
	GLfloat lightAmbientIntensity;
	GLfloat lightDiffuseIntensity;
};

