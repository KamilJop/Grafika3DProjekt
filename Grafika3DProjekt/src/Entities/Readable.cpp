#include "Readable.h"

Readable::Readable(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, SpriteRenderer* spriteRenderer, Texture* texture, bool interaction)
	: Entity(model, position, rotation, scale, interaction), renderer(spriteRenderer), pageTexture(texture)
{
	actionText = "Read";
}	
void Readable::Interact(Inventory* playerInventory)
{
	isReading = true;
}

