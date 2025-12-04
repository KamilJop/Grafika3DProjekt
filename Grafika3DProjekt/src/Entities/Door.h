#pragma once
#include "Entity.h"
class Door : public Entity
{
public:
	bool isOpen;
	Entity* doorFrame;

	Door(Model* model, Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string name, Entity* frame, bool interaction = true);
	~Door();
	void Interact() override ;
	void Update(float deltaTime) override;
};

