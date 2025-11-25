#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
class Player
{
public:
	Player(Camera* cam,glm::vec3 pos = glm::vec3(0.0f), glm::vec3 vel = glm::vec3(0.0f));
	~Player();
	glm::vec3 position;
	glm::vec3 velocity;
	bool isFlashlightOn = false;
	bool onGround = false;
	Camera* camera;
	void UpdatePhysics(float deltaTime);
	void Jump();
	bool getFlashlightState() { return isFlashlightOn; }
	void changeFlashlightState();

};

