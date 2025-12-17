#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Core/Scene.h"


class Drawer : public Entity
{
	public:
	Drawer(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string keyTag = "", bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	void setLocked(bool state) { isLocked = state; }
	~Drawer();
private:
	bool isLocked = true;
	bool isMoved = false;
	bool isMoving = false;
	bool isAnimating = false;
	bool isTryingToOpen = false;
	float animCounter = 0.0;
	glm::vec3 originalPosition;
	glm::vec3 movedPosition;
	std::string drawerKeyTag;

	std::string drawerOpeningSoundPath = "Audio/drawer_opening.mp3";
	std::string drawerClosingSoundPath = "Audio/drawer_closing.mp3";
	std::string drawerLockedSoundPath = "Audio/drawer_locked.mp3";


};

