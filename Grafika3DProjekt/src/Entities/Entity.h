#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Rendering/Shader.h"
#include "Rendering/CollisionBox.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"



class Entity
{
public:

	Model* entityModel;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	bool castsShadow = true;
	bool outlined = false;
	bool hasCollisions = true;
	bool interactable;
	bool shouldGetDestroyed = false;
	CollisionBox collisions;
	std::string title = "Untitled object";

	Entity(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = false);

	~Entity();

	glm::mat4 GetModelMatrix();

	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newRotation);
	void setScale(glm::vec3 newScale);
	void setCastsShadow(bool state) { castsShadow = state; }
	void setTitle(std::string newTitle) { title = newTitle; }
	void setColissions(bool state) { hasCollisions = state; }
	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	bool getInteractable() { return interactable; }
	bool getCastsShadow() { return castsShadow; }
	bool getColissions() { return hasCollisions; }
	std::string getTitle() { return title; }
	bool isOutlined() { return outlined; }
	void setOutlined(bool state) { outlined = state; }
	CollisionBox GetCollisions() { return collisions; }
	void DrawEntity(Shader* shader);
	void UpdateCollisionBox();
	virtual void Update(float deltaTime) {};
	virtual void Interact() {};

private:
	glm::mat4 modelMatrix;
	glm::mat4 CalculateModelMatrix();
	

};

