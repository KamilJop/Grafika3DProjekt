#include "Player.h"

Player::Player(Camera* cam, Entity* flashlight, glm::vec3 pos, glm::vec3 vel)
{
	camera = cam;
	flashlightEntity = flashlight;
	position = pos;
	velocity = vel;
}

Player::~Player()
{
	camera = nullptr;
	position = glm::vec3(0.0f);	
	velocity = glm::vec3(0.0f);
}

void Player::UpdatePhysics(float deltaTime)
{
	updatePlayerCollisions();
	const float gravity = -9.81f;
	velocity.y += gravity * deltaTime;
	position += velocity * deltaTime;
	if (position.y <= 0.0f)
	{
		position.y = 0.0f;
		velocity.y = 0.0f;
		onGround = true;
	}
	else
	{
		onGround = false;
	}

	float targetHeight = isCrouching ? 1.2f : 1.7f;
	float currentHeight = camera->Position.y - position.y;
	float heightDiff = targetHeight - currentHeight;
	float newHeight = glm::mix(currentHeight, targetHeight, 10.0f * deltaTime);

	camera->Position = position + glm::vec3(0.0f, newHeight, 0.0f);
}

void Player::Jump()
{
	if (onGround)
	{
		velocity.y = 5.0f; 
		onGround = false;
	}
}

void Player::Crouch(bool state)
{
	isCrouching = state;
}

void Player::changeFlashlightState()
{
	isFlashlightOn = !isFlashlightOn;
}

void Player::updateFlashlightPosition(const glm::vec3& finalPos)
{
	static float flipTimer = 0.0f;
	flipTimer += 0.032f;
	glm::vec3 finalPosAdjusted = finalPos;
	if (!isFlipping)
	{
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
	else {
		flashlightEntity->setPosition(finalPosAdjusted);
		float pitch = camera->Pitch;
		flashlightEntity->setRotation(glm::vec3(-pitch, 240.0 * flipTimer, 0.0f));
	}
	
}

void Player::flashlightFlipTrick(bool state)
{
	isFlipping = state;
}

void Player::updatePlayerCollisions() {
	float width = 0.6f;
	float height = 1.8f;

	if (isCrouching) height = 1.2f; 

	float halfWidth = width / 2.0f;

	playerCollisions.min = glm::vec3(position.x - halfWidth, position.y, position.z - halfWidth);
	playerCollisions.max = glm::vec3(position.x + halfWidth, position.y + height, position.z + halfWidth);
}