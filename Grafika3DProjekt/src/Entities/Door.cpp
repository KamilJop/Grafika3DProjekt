#include "Door.h"


Door::Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name,Entity* frame, std::string keyTag, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	isOpen = false;
	isAnimating = false;
	isTryingToOpen = false;
	title = name;
	doorFrame = frame;
	doorKeyTag = keyTag;
	AudioManager::GetInstance().Load3DSoundEffect("door_opening", openingSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect("door_closing", closingSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect("door_locked", lockedSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect("door_unlocking", unlockingSoundPath);
}

Door::~Door()
{
	isOpen = false;
	isAnimating = false;
	isTryingToOpen = false;
	title = "";
	doorFrame = nullptr;
}

void Door::Interact(Inventory* playerInventory)
{
	if (isAnimating || isTryingToOpen) return;

	if (isLocked)
	{
		if(playerInventory->GetCurrentItem()->tag == doorKeyTag)
		{
			playerInventory->RemoveItem(doorKeyTag);
			isLocked = false;
			AudioManager::GetInstance().Play3DSoundEffect(unlockingSoundName, position, config.sfxVolume * 2.0f);
			return;
		}

		isTryingToOpen = true;
		animCounter = 0.0;
		AudioManager::GetInstance().Play3DSoundEffect(lockedSoundName, position, config.sfxVolume * 2.0f);
		return;
	}

	isOpen = !isOpen;


	if (isOpen)
	{
		doorFrame->setColissions(false);
		AudioManager::GetInstance().Play3DSoundEffect(openingSoundName,position, config.sfxVolume * 2.0f);
	}
	else
	{
		doorFrame->setColissions(true);
		AudioManager::GetInstance().Play3DSoundEffect(closingSoundName, position, config.sfxVolume * 2.0f);
	}
}


void Door::Update(float deltaTime)
{
	if (isTryingToOpen) {
		animCounter += deltaTime;
		if (animCounter < 0.5f) {
			float shakeOffset = sin(animCounter * 30.0f) * 2.0f;
			rotation.y = shakeOffset;
		}
		else {
			rotation.y = 0.0f; 
			isTryingToOpen = false;
			animCounter = 0.0f;
		}
		return;
	}
	isAnimating = false;
	isTryingToOpen = false;
	animCounter = 0;
	float targetAngle = isOpen ? 90.0f : 0.0f;
	float currentAngle = rotation.y;
	if (abs(currentAngle - targetAngle) > 0.8f)
		isAnimating = true;
	else
		isAnimating = false;
	float newAngle = glm::mix(currentAngle, targetAngle, 5.0f * deltaTime);
	rotation.y = newAngle;
	UpdateCollisionBox();
}

