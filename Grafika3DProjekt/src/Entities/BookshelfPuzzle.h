#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"
#include "Book.h"
class BookshelfPuzzle : public Entity
{
public:
	BookshelfPuzzle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = false);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	~BookshelfPuzzle();
	
private:
	bool isMoving = false;
	bool ended = false;
	glm::vec3 originalPosition;
	glm::vec3 solvedPosition;
};

