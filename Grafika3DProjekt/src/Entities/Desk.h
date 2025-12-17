#pragma once
#include "Entity.h"
#include "Drawer.h"
#include "Door.h"
#include "../Systems/AudioManager.h"
#include <string>
#include <vector>
#include "../Core/Scene.h"

class Desk : public Entity
{
public:
	Desk(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::vector<Model*> drawerModels, Model* doorModel, Scene* scene, bool interaction);
	~Desk();
	void Interact(Inventory* playerInventory) override {};



private:
	std::vector<Drawer*> drawers;
	Door* deskDoor;
};

