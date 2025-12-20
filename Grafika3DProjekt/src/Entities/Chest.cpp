#include "Chest.h"

Chest::Chest(Model* lowerPart, Model* upperPartModel, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, Scene* scene, bool interaction)
	: Entity(lowerPart, pos, rot, scal, interaction)
{
	upperPart = new Entity(upperPartModel, pos + glm::vec3(0.0f, 0.55f, 0.45f), rot, scal, false);
	scene->AddEntity(upperPart);
	targetRotation = rot.x + 80.0f;
	AudioManager::GetInstance().Load3DSoundEffect(chestOpenSound, chestOpenSound);
}

void Chest::Interact(Inventory* playerInventory)
{
	if (isLocked) {
		if(!triedToOpen) {
			triedToOpen = true;
			UI::SetSubtitle("The chest is locked. I need to find password to padlock.", 2.0f);
		}
		return;
	}
	isOpen = true;
	AudioManager::GetInstance().Play3DSoundEffect(chestOpenSound, getPosition());
	
}	

void Chest::Update(float deltaTime)
{
	if (!isOpen) return;
	if(upperPart->getRotation().x < targetRotation) {
		float rotationSpeed = 20.0f;
		float newRotationX = upperPart->getRotation().x + rotationSpeed * deltaTime;
		if (newRotationX > targetRotation) {
			newRotationX = targetRotation;
		}
		upperPart->setRotation(glm::vec3(newRotationX, upperPart->getRotation().y, upperPart->getRotation().z));
		interactable = false;
		outlined = false;
	}
}
