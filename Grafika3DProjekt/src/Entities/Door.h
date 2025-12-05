#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include "../Core/Config.h"
#include <string>
#include <math.h>
class Door : public Entity
{
public:
	bool isOpen;
	bool isAnimating;
	bool isTryingToOpen;
	bool isLocked = false;
	std::string doorKeyTag;
	Entity* doorFrame;
	Config& config = Config::getInstance();
	std::string openingSoundPath = "Audio/door_opening.mp3";
	std::string closingSoundPath = "Audio/door_closing.mp3";
	std::string lockedSoundPath = "Audio/door_locked.mp3";
	std::string unlockingSoundPath = "Audio/door_unlocking.mp3";
	std::string openingSoundName = "door_opening";
	std::string closingSoundName = "door_closing";
	std::string lockedSoundName = "door_locked";
	std::string unlockingSoundName = "door_unlocking";

	float animCounter = 0.0;
	Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name, Entity* frame, std::string keyTag = "No needed",  bool interaction = true);
	~Door();
	void setLocked(bool state) { isLocked = state; }
	void Interact(Inventory* playerInventory) override ;
	void Update(float deltaTime) override;
};

