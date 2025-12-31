#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Model.h"
#include "../Systems/CandlePuzzle.h"
class HauntedEntity : public Entity
{
	public:
	HauntedEntity(Model* model,Model* secondModel, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = true);
	void Update(float deltaTime) override ;
	std::string GetActionText() override { return "Examine"; };
	~HauntedEntity();
private:
	Model* alternateModel;
	bool hasSwitched = false;
};

