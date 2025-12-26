#include "Pickable.h"


Pickable::Pickable(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, Texture* tex, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	itemTag = tag;
	itemTexture = tex;
	pickable = true;
	AudioManager::GetInstance().Load3DSoundEffect(pickupSoundPath, pickupSoundPath);
}

void Pickable::Interact(Inventory* playerInventory)
{
	AudioManager::GetInstance().Play3DSoundEffect(pickupSoundPath, position, 1.0f);
	isPickedUp = true;
	UI::SetSubtitle("Item picked up.", 2.0f);
}

void Pickable::Update(float deltaTime)
{
	if (isPickedUp) {
		shouldGetDestroyed = true;
	}
}

Pickable::~Pickable()
{
	itemTexture = nullptr;
	isPickedUp = false;
}