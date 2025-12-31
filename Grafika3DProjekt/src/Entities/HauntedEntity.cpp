#include "HauntedEntity.h"


HauntedEntity::HauntedEntity(Model* model, Model* secondModel, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction), alternateModel(secondModel)
{
	alternateModel = secondModel;
}




void HauntedEntity::Update(float deltaTime)
{
	hasExamined = false;
	if (hasSwitched) return;
	if (CandlePuzzle::isSolved) {
		entityModel = alternateModel;
		hasSwitched = true;
	}
}