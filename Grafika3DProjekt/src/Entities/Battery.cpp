#include "Battery.h"

Battery::Battery(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, Texture* tex, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	itemTag = tag;
	itemTexture = tex;
	pickable = true;
	AudioManager::GetInstance().Load3DSoundEffect(pickupSoundName, pickupSoundPath);
}

void Battery::Interact(Inventory* playerInventory)
{
	AudioManager::GetInstance().Play3DSoundEffect(pickupSoundName, position, 1.0f);
	isPickedUp = true;
	UI::SetSubtitle("I wonder what this opens.", 2.0f);
}

void Battery::Update(float deltaTime)
{
	if (isPickedUp) {
		shouldGetDestroyed = true;
	}
}

Battery::~Battery()
{
	itemTexture = nullptr;
	isPickedUp = false;
}