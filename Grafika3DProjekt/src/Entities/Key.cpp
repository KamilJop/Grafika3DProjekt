#include "Key.h"

Key::Key(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, std::string image, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	itemTag = tag;
	itemImagePath = image;
	pickable = true;
	AudioManager::GetInstance().Load3DSoundEffect(pickupSoundName, pickupSoundPath);
}

void Key::Interact(Inventory* playerInventory)
{
	AudioManager::GetInstance().Play3DSoundEffect(pickupSoundName, position, 1.0f);
	isPickedUp = true;
}

void Key::Update(float deltaTime)
{
	if (isPickedUp) {
		shouldGetDestroyed = true;
	}
}