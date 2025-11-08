#pragma once
#include "Light.h"
class PointLight : protected Light
{
public:
    PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensit,glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad);
	~PointLight();
	void useLight(Shader* lightShader);
	glm::vec3 getColor() { return lightColor; }
	glm::vec3 getPosition() { return lightPosition; }
protected:
	glm::vec3 lightPosition;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
};

