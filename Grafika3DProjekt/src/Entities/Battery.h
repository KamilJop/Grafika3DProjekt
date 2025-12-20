#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"
class Battery : public Entity
{
public:
	Battery(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, Texture* tex, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	Texture* getTexture()override { return itemTexture; }
	std::string GetActionText() override { return "Pick Up"; };
	~Battery();
private:
	bool isPickedUp = false;
	Texture* itemTexture;
	std::string pickupSoundPath = "Audio/key_pickup.mp3";
	std::string pickupSoundName = "key_pickup";
};

