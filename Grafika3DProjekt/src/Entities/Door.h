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
	bool isLocked = false;;
	Entity* doorFrame;
	Config& config = Config::getInstance();
	std::string openingSoundPath = "Audio/door_opening.mp3";
	std::string closingSoundPath = "Audio/door_closing.mp3";
	std::string lockedSoundPath = "Audio/door_locked.mp3";
	std::string openingSoundName = "door_opening";
	std::string closingSoundName = "door_closing";
	std::string lockedSoundName = "door_locked";
	float animCounter = 0.0;
	Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name, Entity* frame,  bool interaction = true);
	~Door();
	void setLocked(bool state) { isLocked = state; }
	void Interact() override ;
	void Update(float deltaTime) override;
};

