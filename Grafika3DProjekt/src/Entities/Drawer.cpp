#include "Drawer.h"


Drawer::Drawer(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string keyTag, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	originalPosition = position;
	drawerKeyTag = keyTag;
	AudioManager::GetInstance().Load3DSoundEffect(drawerOpeningSoundPath, drawerOpeningSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect(drawerClosingSoundPath, drawerClosingSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect(drawerLockedSoundPath, drawerLockedSoundPath);
}


void Drawer::Interact(Inventory* playerInventory)
{
	if (isMoving || isAnimating) return;
	if (isLocked)
	{
		if (playerInventory->GetCurrentItem()->tag == drawerKeyTag)
		{
			playerInventory->RemoveItem(drawerKeyTag);
			isLocked = false;
			AudioManager::GetInstance().Play3DSoundEffect(drawerLockedSoundPath, position, Config::getInstance().sfxVolume * 2.0f);
			return;
		}

		isTryingToOpen = true;
		animCounter = 0.0;
		AudioManager::GetInstance().Play3DSoundEffect(drawerLockedSoundPath, position, Config::getInstance().sfxVolume * 2.0f);
		return;
	}
	if (isMoved)
	{
		movedPosition = originalPosition;
		AudioManager::GetInstance().Play3DSoundEffect(drawerClosingSoundPath, position, Config::getInstance().sfxVolume * 2.0f);
	}
	else
	{
		movedPosition = originalPosition + glm::vec3(0.0f, 0.0f, 0.3f);
		AudioManager::GetInstance().Play3DSoundEffect(drawerOpeningSoundPath, position, Config::getInstance().sfxVolume * 2.0f);
	}
	isMoving = true;
}

void Drawer::Update(float deltaTime)
{
	if (isTryingToOpen) {
		animCounter += deltaTime;
		if (animCounter < 0.35f) {
			float shakeOffset = sin(animCounter * 15.0f) * 2.0f;
			rotation.x = shakeOffset;
		}
		else {
			rotation.x = 0.0f;
			isTryingToOpen = false;
			animCounter = 0.0f;
		}
		return;
	}
	isAnimating = false;
	isTryingToOpen = false;
	animCounter = 0;
	if (isMoving)
	{
		float moveSpeed = 3.0f; 
		{
			position = glm::mix(position, movedPosition, moveSpeed * deltaTime);
			UpdateCollisionBox();
			if (glm::length(position - movedPosition) < 0.03f)
			{
				position = movedPosition;
				isMoving = false;
				isMoved = !isMoved;
			}
		}
	}
}