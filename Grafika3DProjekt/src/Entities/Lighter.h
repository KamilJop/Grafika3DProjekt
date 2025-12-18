#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"
class Lighter : public Entity
{
public:
	Lighter(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,Texture* tex, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	Texture* getTexture()override { return itemTexture; }
	std::string GetActionText() override { return "Pick Up"; };
	~Lighter();
private:
	bool isPickedUp = false;
	Texture* itemTexture;
};

