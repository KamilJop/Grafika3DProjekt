#include "Player.h"

Player::Player(Camera* cam, Entity* flashlight, glm::vec3 pos, glm::vec3 vel)
{
	camera = cam;
	flashlightEntity = flashlight;
	position = pos;
	velocity = vel;
	playerInventory = Inventory();
	updatePlayerCollisions();
}

Player::~Player()
{
	camera = nullptr;
	position = glm::vec3(0.0f);	
	velocity = glm::vec3(0.0f);
}

void Player::UpdatePhysics(float deltaTime, std::vector<Entity*>& entities)
{
	// Remember position before checking collisions
	float previousX = position.x;
	float previousY = position.y;
	float previousZ = position.z;

	// Apply gravity
	const float gravity = -9.81f;
	if (!onGround) 	velocity.y += gravity * deltaTime;

	// X movement
	position.x += velocity.x * deltaTime;
	updatePlayerCollisions();
	
	// Check for collisions on X axis
	for (auto& entity : entities)
	{
		CollisionBox otherBox = entity->GetCollisions();
		// Skip flashlight entity
		if (entity == flashlightEntity) continue;
		if (!entity->getColissions()) continue;

		if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
			playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
			playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
		{
			// Collision detected go back to previous X
			position.x = previousX;
			velocity.x = 0.0f;
			updatePlayerCollisions(); 
			break;
		}
	}

	// Z movement
	position.z += velocity.z * deltaTime;
	updatePlayerCollisions();
	// Check for collisions on Z axis
	for (auto& entity : entities)
	{
		// Skip flashlight entity
		if (entity == flashlightEntity) continue;
		if (!entity->getColissions()) continue;
		CollisionBox otherBox = entity->GetCollisions();
		if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
			playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
			playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
		{
			// Collision detected go back to previous Z
			position.z = previousZ;
			velocity.z = 0.0f;
			updatePlayerCollisions(); 
			break;
		}
	}

	// Y movement
	position.y += velocity.y * deltaTime;
	updatePlayerCollisions();

	bool onObject = false;

	for (auto & entity : entities)
	{
		// Skip flashlight entity
		if (entity == flashlightEntity) continue;
		if (!entity->getColissions()) continue;
		CollisionBox otherBox = entity->GetCollisions();
		if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
			playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
			playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
		{
			// Collision detected go back to previous Y
			onObject = true;
			position.y = previousY;
			velocity.y = 0.0f;
			updatePlayerCollisions(); 
			break;
		}
	}

	if (onObject)
	{
		onGround = true;
	}
	else {
		onGround = false;
	}


	float targetHeight = isCrouching ? 1.0f : 1.5f;
	float currentHeight = camera->Position.y - position.y;
	float heightDiff = targetHeight - currentHeight;
	float newHeight = glm::mix(currentHeight, targetHeight, 10.0f * deltaTime);

	camera->Position = position + glm::vec3(0.0f, newHeight, 0.0f);
}

void Player::Jump()
{
	if (onGround)
	{
		velocity.y = 6.0f; 
		onGround = false;
	}
}

void Player::Crouch(bool state)
{
	isCrouching = state;
}

void Player::changeFlashlightState(bool state)
{
	isFlashlightOn = state;
}

void Player::updateFlashlightPosition(const glm::vec3& finalPos)
{
	glm::vec3 finalPosAdjusted = finalPos;

	if (walkTimer > 0.0f)
	{
		float bobFrequency = 10.0f;
		float bobAmountX = 0.015f;
		float bobAmountY = 0.005f;
		float bobX = cos(walkTimer * bobFrequency / 2.0f) * bobAmountX;
		float bobY = sin(walkTimer * bobFrequency) * bobAmountY;
		finalPosAdjusted += camera->Right * bobX;
		finalPosAdjusted += camera->Up * bobY;
	}
	flashlightEntity->setPosition(finalPosAdjusted);
	float pitch = camera->Pitch;
	float yaw = camera->Yaw;
	flashlightEntity->setRotation(glm::vec3(-pitch, -yaw + 90.0f, 0.0f));
}
	


void Player::updatePlayerCollisions() {
	float width = 0.7f;
	float height = 1.8f;

	if (isCrouching) height = 1.8f; 

	float halfWidth = width / 2.0f;

	playerCollisions.min = glm::vec3(position.x - halfWidth, position.y, position.z - halfWidth);
	playerCollisions.max = glm::vec3(position.x + halfWidth, position.y + height, position.z + halfWidth);

}

void Player::checkTargettedEntity(std::vector<Entity*>& entities)
{
	// Raycasting parameters
	float reachDistance = 3.0f;
	glm::vec3 rayOrigin = camera->getCameraPosition();
	glm::vec3 rayDirection = camera->getCameraFront();
	Entity* closestEntity = nullptr;
	targettedEntity = nullptr;
	float closestDistance = reachDistance;
	// Check intersection with each entity
	for (auto& entity : entities)
	{
		// Skip flashlight entity
		if (entity == flashlightEntity) continue;
		// Only consider interactable entities
		if (!entity->getInteractable()) continue;
		entity->setOutlined(false);
		float intersectionDistance = 10000.0f;
		if (checkRayEntityIntersection(rayOrigin, rayDirection, entity, intersectionDistance))
		{
			if (intersectionDistance < closestDistance)
			{
				closestDistance = intersectionDistance;
				closestEntity = entity;
			}
		}
	}
	// Outline the closest entity if any
	if (closestEntity)
	{
		closestEntity->setOutlined(true);
		targettedEntity = closestEntity;
	}
}


bool Player::checkRayEntityIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Entity* entity, float& outDistance)
{
	CollisionBox box = entity->GetCollisions();

	float tx1 = (box.min.x - rayOrigin.x) / rayDirection.x;
	float tx2 = (box.max.x - rayOrigin.x) / rayDirection.x;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (box.min.y - rayOrigin.y) / rayDirection.y;
	float ty2 = (box.max.y - rayOrigin.y) / rayDirection.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	float tz1 = (box.min.z - rayOrigin.z) / rayDirection.z;
	float tz2 = (box.max.z - rayOrigin.z) / rayDirection.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	if (tmax >= tmin && tmin >= 0.0f)
	{
		outDistance = tmin;
		return true;
	}
	return false;
}

void Player::pickUpEntity(Entity* entity)
{
	if (entity->getInteractable())
	{
		playerInventory.AddItem(entity->getTag(),entity->getTitle(), entity->getTexture());
		entity->shouldGetDestroyed = true;
	}
}