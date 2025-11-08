#include "Light.h"


// Constructor
Light::Light(glm::vec3 colors = glm::vec3(1.0f), GLfloat ambientIntensity = 0.1f, GLfloat diffuseIntensity = 0.1f)
{
	lightColor = colors;
	lightAmbientIntensity = ambientIntensity;
	lightDiffuseIntensity = diffuseIntensity;
}

// Destructor
Light::~Light()
{
	lightColor = glm::vec3(0.0f);
	lightAmbientIntensity = 0.0f;
	lightDiffuseIntensity = 0.0f;
}

