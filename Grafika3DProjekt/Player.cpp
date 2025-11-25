#include "Player.h"

Player::Player(Camera* cam, glm::vec3 pos, glm::vec3 vel)
{
	camera = cam;
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