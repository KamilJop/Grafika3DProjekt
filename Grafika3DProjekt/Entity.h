#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include "Model.h"
#include "Material.h"



class Entity
{
public:

	Model* entityModel;
	Material* entityMaterial;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;


	Entity(Model* model, Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal);

	~Entity();

	glm::mat4 GetModelMatrix();

	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newRotation);
	void setScale(glm::vec3 newScale);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	void DrawEntity(Shader* shader);
	virtual void Update(float deltaTime) {};

private:
	glm::mat4 modelMatrix;
	glm::mat4 CalculateModelMatrix();
	

};

