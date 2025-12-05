#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
class Key : public Entity
{
public:
	Key(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Interact() override;
	void Update(float deltaTime) override;
	~Key();
private:
	bool isPickedUp = false;
	std::string pickupSoundPath = "Audio/key_pickup.mp3";
	std::string pickupSoundName = "key_pickup";
};

