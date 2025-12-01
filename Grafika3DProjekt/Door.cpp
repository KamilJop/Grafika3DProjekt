#include "Door.h"


Door::Door(Model* model, Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name, bool interaction)
	: Entity(model, material, pos, rot, scal, interaction)
{
	isOpen = false;
	title = name;
}

Door::~Door()
{
	Entity::~Entity();
}

void Door::Interact()
{
	isOpen = !isOpen;
}


void Door::Update(float deltaTime)
{
	float targetAngle = isOpen ? -90.0f : 0.0f;
	float currentAngle = rotation.y;
	float newAngle = glm::mix(currentAngle, targetAngle, 5.0f * deltaTime);
	rotation.y = newAngle;
}

