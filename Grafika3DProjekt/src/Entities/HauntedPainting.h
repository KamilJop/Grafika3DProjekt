#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Model.h"
class HauntedPainting : public Entity
{
	public:
	HauntedPainting(Model* model,Model* secondModel, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override {};
	std::string GetActionText() override { return "Examine"; };
	~HauntedPainting();
private:
	Model* alternateModel;
};

