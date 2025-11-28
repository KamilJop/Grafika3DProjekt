#pragma once
#include <glm/glm.hpp>
#include "Camera.h"
#include "Entity.h"
#include "CollisionBox.h"
#include <vector>

class Player
{
public:
	Player(Camera* cam, Entity* flashlight, glm::vec3 pos = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 vel = glm::vec3(0.0f));
	~Player();
	glm::vec3 position;
	glm::vec3 velocity;
	Entity* flashlightEntity;
	float walkTimer = 0.0f;
	bool isFlashlightOn = false;
	bool onGround = false;
	bool isCrouching = false;
	Camera* camera;
	CollisionBox playerCollisions;


	void UpdatePhysics(float deltaTime, std::vector<Entity*>& entities);
	void checkTargettedEntity(std::vector<Entity*>& entities);
	bool checkRayEntityIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Entity* entity, float& outDistance);
	void Jump();
	void changeFlashlightState();
	void Crouch(bool state);
	bool getFlashlightState() { return isFlashlightOn; }
	void updateFlashlightPosition(const glm::vec3& finalPos);
	void updatePlayerCollisions();
	CollisionBox GetPlayerCollisions() { return playerCollisions; };
	

};

