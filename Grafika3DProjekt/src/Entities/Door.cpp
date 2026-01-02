#include "Door.h"


Door::Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name,Entity* frame, std::string keyTag, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	isOpen = false;
	isAnimating = false;
	isTryingToOpen = false;
	title = name;
	startRotationY = rot.y;
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

		if(!triedToOpen) {
			triedToOpen = true;
				
			UI::SetSubtitle(examineText,3.0f);
		}
		isTryingToOpen = true;
		animCounter = 0.0;
		AudioManager::GetInstance().Play3DSoundEffect(lockedSoundName, position, config.sfxVolume * 2.0f);
		return;
	}

	isOpen = !isOpen;


	if (isOpen)
	{
		if (doorFrame != nullptr) {
			doorFrame->setColissions(false);
		}
		AudioManager::GetInstance().Play3DSoundEffect(openingSoundName,position, config.sfxVolume * 2.0f);
	}
	else
	{	
		if (doorFrame != nullptr) {
			doorFrame->setColissions(true);
		}
		AudioManager::GetInstance().Play3DSoundEffect(closingSoundName, position, config.sfxVolume * 2.0f);
	}
}


void Door::Update(float deltaTime)
{
	if (isTryingToOpen) {
		animCounter += deltaTime;
		if (animCounter < 0.5f) {
			float shakeOffset = sin(animCounter * 30.0f) * 2.0f;
			rotation.y = startRotationY + shakeOffset;
		}
		else {
			rotation.y = startRotationY; 
			isTryingToOpen = false;
			animCounter = 0.0f;
		}
		return;
	}
	isAnimating = false;
	isTryingToOpen = false;
	animCounter = 0;
	float targetAngle = isOpen ? (startRotationY + (90.0f * direction)) : startRotationY;
	float currentAngle = rotation.y;
	if (abs(currentAngle - targetAngle) > 0.8f)
		isAnimating = true;
	else
		isAnimating = false;
	float newAngle = glm::mix(currentAngle, targetAngle, 5.0f * deltaTime);
	rotation.y = newAngle;
	UpdateCollisionBox();
}


std::string Door::GetActionText()
{
	if (isOpen) {
		return "Close Door";
	}
	else {
		return "Open Door";
	}
}
