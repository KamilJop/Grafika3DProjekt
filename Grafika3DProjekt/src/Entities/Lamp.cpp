#include "Lamp.h"

Lamp::Lamp (Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, PointLight* lightSource, bool interaction)
	: Entity(model, position, rotation, scale, interaction), light(lightSource) 
{
	actionText = "Toggle Lamp";
	targetAmbient = light->getAmbientIntensity();
	targetDiffuse = light->getDiffuseIntensity();
	targetColor = light->getColor();
	light->setAmbientIntensity(0.0f);
	light->setDiffuseIntensity(0.0f);
	light->setColor(glm::vec3(0.0f));
	light->setPosition(position + glm::vec3(0.0f, 3.0f, 0.0f));
	Interact(nullptr);
}

void Lamp::Interact(Inventory* playerInventory)
{

	if (isOn) {
		light->setAmbientIntensity(0.0f);
		light->setDiffuseIntensity(0.0f);
		light->setColor(glm::vec3(0.0f));
		isOn = false;
	}
	else {
		light->setAmbientIntensity(targetAmbient);
		light->setDiffuseIntensity(targetDiffuse);
		light->setColor(targetColor);
		isOn = true;
	}
}