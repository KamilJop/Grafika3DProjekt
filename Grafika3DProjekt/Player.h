#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Entity.h"

class Player
{
public:
	Player(Camera* cam, Entity* flashlight, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 vel = glm::vec3(0.0f));
	~Player();
	glm::vec3 position;
	glm::vec3 velocity;
	Entity* flashlightEntity;
	bool isFlashlightOn = false;
	bool onGround = false;
	bool isCrouching = false;
	Camera* camera;
	void UpdatePhysics(float deltaTime);
	void Jump();
	void changeFlashlightState();
	void Crouch(bool state);
	bool getFlashlightState() { return isFlashlightOn; }
	void updateFlashlightPosition(const glm::vec3& finalPos);
	

};

