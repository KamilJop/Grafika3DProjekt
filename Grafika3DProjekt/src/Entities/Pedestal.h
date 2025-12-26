#pragma once
#include "Entity.h"
class Pedestal : public Entity
{

	public:
	Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Interact(Inventory* playerInventory) override {};
	void Update(float deltaTime) override {};
	std::string GetActionText() override { return "Examine"; };
};

