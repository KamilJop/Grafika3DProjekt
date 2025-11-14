#pragma once
#include "Light.h"
#include "OmniShadowMap.h"
#include <vector>
class PointLight : protected Light
{
public:
    PointLight(glm::vec3 colors, GLfloat ambientIntensity, GLfloat diffuseIntensit,glm::vec3 lightPos, GLfloat con, GLfloat lin, GLfloat quad, int index = 0, GLfloat far, GLfloat near, GLfloat shadowH, GLfloat shadowW);
	~PointLight();
	void useLight(Shader* lightShader);
	glm::vec3 getColor() { return lightColor; }
	glm::vec3 getPosition() { return lightPosition; }
	std::vector<glm::mat4> calculateLightTransform();
protected:
	glm::vec3 lightPosition;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
	GLfloat farPlane;
	GLfloat nearPlane;
	GLfloat shadowHeight;
	GLfloat shadowWidth;
	OmniShadowMap* shadowMap;
	glm::mat4 lightProjection;
	int lightIndex;
};

