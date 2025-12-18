#include "Lighter.h"

Lighter::Lighter(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,Texture* tex, bool interaction) 
	: Entity(model, pos, rot, scal, interaction) {
	pickable = true;
	isPickedUp = false;
	itemTexture = tex;
	itemTag = "Lighter";
}

void Lighter::Interact(Inventory* playerInventory)
{
	isPickedUp = true;
	UI::SetSubtitle("Let's burn this place.", 2.0f);
}

void Lighter::Update(float deltaTime)
{
	if (isPickedUp) {
		shouldGetDestroyed = true;
	}
}