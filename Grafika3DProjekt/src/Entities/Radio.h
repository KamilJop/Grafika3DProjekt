#pragma once
#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

enum RadioState
{
	BROKEN,
	WRONG_FREQUENCY,
	PLAYING,
	OFF
};

class Radio : public Entity
{
public:
	Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override {};
	~Radio();
private:
	int currentState = BROKEN;
	std::string wrongFrequencySoundPath = "Audio/broken_radio.mp3";
};

