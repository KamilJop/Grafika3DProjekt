#include "Door.h"


Door::Door(Model* model, Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name,Entity* frame, bool interaction)
	: Entity(model, material, pos, rot, scal, interaction)
{
	isOpen = false;
	isAnimating = false;
	title = name;
	doorFrame = frame;
	audioManager.Load3DSoundEffect("door_opening", openingSoundPath);
	audioManager.Load3DSoundEffect("door_closing", closingSoundPath);
}

Door::~Door()
{
	Entity::~Entity();
}

void Door::Interact()
{
	if(isAnimating)
		return;
	isOpen = !isOpen;
	if (isOpen)
	{
		doorFrame->setColissions(false);
		audioManager.Play3DSoundEffect(openingSoundName,position, config.sfxVolume * 2.0);
	}
	else
	{
		doorFrame->setColissions(true);
		audioManager.Play3DSoundEffect(closingSoundName, position, config.sfxVolume * 2.0);
	}
}


void Door::Update(float deltaTime)
{
	float targetAngle = isOpen ? -90.0f : 0.0f;
	float currentAngle = rotation.y;
	if (abs(currentAngle - targetAngle) > 0.8f)
		isAnimating = true;
	else
		isAnimating = false;
	float newAngle = glm::mix(currentAngle, targetAngle, 5.0f * deltaTime);
	rotation.y = newAngle;
	UpdateCollisionBox();
}

