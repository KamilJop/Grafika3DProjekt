#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
class Key : public Entity
{
public:
	Key(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, std::string image, bool interaction = true );
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	~Key();
private:
	bool isPickedUp = false;
	std::string itemImagePath;
	std::string pickupSoundPath = "Audio/key_pickup.mp3";
	std::string pickupSoundName = "key_pickup";
};

