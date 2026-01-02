#pragma once
#include "Entity.h"
#include "../Core/Inventory.h"
#include "../Core/Scene.h"
#include "../Light/PointLight.h"
class Pedestal : public Entity
{

public:
	Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, Scene* scene,PointLight* pl,bool interaction = false);
	void Interact(Inventory* playerInventory) override ;
	void Update(float deltaTime) override ;
	std::string GetActionText() override { return "Examine"; };
	bool getHasCorrectItem() { return hasCorrectItem; }
	void setCorrectItemTag(const std::string& tag) { correctItemTag = tag; }
private:
	Entity* pedestalEntity;
	Item* storedItem;
	Scene* currentScene;
	PointLight* pedestalLight;
	bool isItemPlaced = false;
	bool hasCorrectItem = false;
	std::string correctItemTag = "";
};

