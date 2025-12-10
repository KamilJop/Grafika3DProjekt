#pragma once
#define NOMINMAX
#include <Windows.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Entities/Entity.h"
#include "Rendering/CollisionBox.h"
#include <vector>
#include "Inventory.h"

class Player
{
public:
	Player(Camera* cam, Entity* flashlight, glm::vec3 pos = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 vel = glm::vec3(0.0f));
	~Player();
	glm::vec3 position;
	glm::vec3 velocity;
	Entity* flashlightEntity;
	Entity* heldEntity;
	Entity* targettedEntity = nullptr;
	float walkTimer = 0.0f;
	bool isFlashlightOn = false;
	bool onGround = false;
	bool isCrouching = false;
	Camera* camera;
	CollisionBox playerCollisions;
	Inventory playerInventory;


	void UpdatePhysics(float deltaTime, std::vector<Entity*>& entities);
	void checkTargettedEntity(std::vector<Entity*>& entities);
	bool checkRayEntityIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Entity* entity, float& outDistance);
	void Jump();
	void changeFlashlightState(bool state);
	void Crouch(bool state);
	bool getFlashlightState() { return isFlashlightOn; }
	Inventory* getInventory() { return &playerInventory; };
	void updatePlayerCollisions();
	void setHeldEntityModel(Model* model) { heldEntity->entityModel = model; };
	void setHeldEntityScale(glm::vec3 scale) { heldEntity->setScale(scale); };
	void updateHeldEntityPosition(const glm::vec3& finalPos);
	void pickUpEntity(Entity* entity);
	Entity* getTargettedEntity() { return targettedEntity; };
	bool hasTargettedEntity() { return targettedEntity != nullptr; };
	CollisionBox GetPlayerCollisions() { return playerCollisions; };
	

};

