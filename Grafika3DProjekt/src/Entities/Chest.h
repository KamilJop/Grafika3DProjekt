#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Core/Scene.h"
class Chest : public Entity
{
	public:
	Chest(Model* lowerPart, Model*upperPart, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, Scene* scene, bool interaction = true);
	~Chest();
	void Interact(Inventory* playerInventory) override ;
	void Update(float deltaTime) override ;
	void OpenChest() {isLocked = false;};

private:
	Entity* upperPart;
	bool triedToOpen = false;
	bool isLocked = true;
	bool isOpen = false;
	float targetRotation = 0.0f;
	std::string chestOpenSound = "Audio/chest_opening.mp3";
};

