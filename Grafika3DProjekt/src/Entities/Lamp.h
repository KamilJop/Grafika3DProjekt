#pragma once
#include "Entity.h"
#include "../Light/PointLight.h"
class Lamp : public Entity
{
	public:
	Lamp(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, PointLight* lightSource, bool interaction);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override {};
		

private:
	PointLight* light;
	bool isOn = false;
	float targetAmbient; 
	float targetDiffuse;
	glm::vec3 targetColor;
};

