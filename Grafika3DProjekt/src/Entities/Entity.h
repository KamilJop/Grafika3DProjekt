#pragma once
#define NOMINMAX
#include <Windows.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Rendering/Shader.h"
#include "Rendering/CollisionBox.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "../Core/Inventory.h"



class Entity
{
public:

	Entity(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = false);

	~Entity();

	glm::mat4 GetModelMatrix();

	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newRotation);
	void setScale(glm::vec3 newScale);
	void setCastsShadow(bool state) { castsShadow = state; }
	void setTitle(std::string newTitle) { title = newTitle; }
	void setColissions(bool state) { hasCollisions = state; }
	void setModel(Model* model) { entityModel = model; UpdateCollisionBox(); }
	void setShouldGetDestroyed(bool state) { shouldGetDestroyed = state; }
	
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	bool getInteractable() { return interactable; }
	bool getCastsShadow() { return castsShadow; }
	bool getColissions() { return hasCollisions; }
	bool getPickable() { return pickable; }
	bool getShouldGetDestroyed() { return shouldGetDestroyed; }
	virtual Texture* getTexture() { return nullptr; };
	std::string getTag() { return itemTag; }	
	std::string getTitle() { return title; }
	Model* getModel() { return entityModel; }	
	bool isOutlined() { return outlined; }
	void setOutlined(bool state) { outlined = state; }
	CollisionBox GetCollisions() { return collisions; }
	void DrawEntity(Shader* shader);
	void UpdateCollisionBox();
	virtual void Update(float deltaTime) {};
	virtual void Interact(Inventory * playerInventory) {};

protected:
	Model* entityModel;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool castsShadow = true;
	bool outlined = false;
	bool hasCollisions = true;
	bool interactable;
	bool shouldGetDestroyed = false;
	bool pickable = false;
	std::string itemTag = "";
	CollisionBox collisions;
	std::string title = "Untitled object";

	glm::mat4 modelMatrix;
	glm::mat4 CalculateModelMatrix();
	

};

