#pragma once

#include <glad/glad.h>
#include "Shader.h"


class Material
{
public:
	Material(GLfloat intensity, GLfloat shine);
	~Material();
	void useMaterial(Shader* shader);
	
private:
	GLfloat specularIntensity;
	GLfloat shininess;
};


