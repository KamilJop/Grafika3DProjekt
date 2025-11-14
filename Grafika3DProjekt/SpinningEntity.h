#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
class SpinningEntity : public Entity
{
	public:
	SpinningEntity(Model* model, Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal)	: Entity(model, material, pos, rot, scal) {};
	virtual void Update(float deltaTime) override;

};

