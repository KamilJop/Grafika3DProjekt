#pragma once
#include "Entity.h"
#include "../Rendering/Texture.h"
class Pickable : public Entity
{
	public:
	Pickable(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, Texture* tex, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	Texture* getTexture()override { return itemTexture; }
	std::string GetActionText() override { return "Pick Up"; };
	~Pickable();
private:
	bool isPickedUp = false;
	Texture* itemTexture;
	std::string pickupSoundPath = "Audio/item_pickup.mp3";

};



