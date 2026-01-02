#pragma once
#include "Entity.h"
#include "../Systems/SpriteRenderer.h"
class Readable : public Entity
{
	public:
	Readable(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, SpriteRenderer* spriteRenderer,Texture* texture, bool interaction);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override {};
	void setReadingState(bool state) { isReading = state; }
	Texture* getPageTexture() { return pageTexture; }
private:
	SpriteRenderer* renderer;
	Texture* pageTexture;
	bool isReading = false;
	std::string readingSoundPath = "Audio/page_flip.mp3";
};

