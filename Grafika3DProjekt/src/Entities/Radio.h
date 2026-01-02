#pragma once
#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

enum RadioState
{
	BROKEN,
	ONE_BATTERY,
	WORKING
};

class Radio : public Entity
{
public:
	Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	std::string GetActionText() override;
	~Radio();
private:
	int currentState = BROKEN;
	float soundTimer = 0.0f;
	std::string brokenRadioSound = "Audio/broken_radio.mp3";
	std::string firstInsert = "Audio/insert_first_battery.mp3";
	std::string secondInsert = "Audio/insert_second_battery.mp3";
	std::string radioNoises = "Audio/radio_noises.mp3";
	std::string morseCodeSound = "Audio/morse.mp3";
};

