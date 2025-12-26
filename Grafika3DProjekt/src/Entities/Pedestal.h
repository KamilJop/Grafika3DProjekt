#pragma once
#include "Entity.h"
#include "../Core/Inventory.h"
#include "../Core/Scene.h"
class Pedestal : public Entity
{

public:
	Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, Scene* scene, bool interaction = false);
	void Interact(Inventory* playerInventory) override ;
	void Update(float deltaTime) override ;
	std::string GetActionText() override { return "Examine"; };

private:
	Entity* pedestalEntity;
	Item* storedItem;
	Scene* currentScene;
	bool isItemPlaced = false;
};

