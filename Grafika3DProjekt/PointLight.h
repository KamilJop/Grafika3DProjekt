#pragma once
#include "Light.h"
class PointLight : Light
{
public:
    PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensit,glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad);
	~PointLight();
	void useLight(Shader* lightShader);
private:
	glm::vec3 lightPosition;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
};

