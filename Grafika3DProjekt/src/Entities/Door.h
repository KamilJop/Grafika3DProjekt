#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include "../Core/Config.h"
#include <string>
#include <math.h>
class Door : public Entity
{
public:



	Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name, Entity* frame, std::string keyTag = "No needed",  bool interaction = true);
	~Door();
	void setLocked(bool state) { isLocked = state; }
	void setIsOpen(bool state) { isOpen = state; }
	void Interact(Inventory* playerInventory) override ;
	void setTriedToOpen(bool state) { triedToOpen = state; }	
	void setExamineText(const std::string& text) { examineText = text; }
	void Update(float deltaTime) override;
	void setDirection(int dir) { direction = dir; }	
	std::string GetActionText() override;
private:
	bool isOpen;
	bool isAnimating;
	bool isTryingToOpen;
	bool triedToOpen = false;
	float startRotationY = 0.0f;
	bool isLocked = false;
	int direction = 1;
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
	std::string examineText = "The door is locked.I probably need to find a key.";
	std::string unlockingSoundName = "door_unlocking";
	float animCounter = 0.0;
};

