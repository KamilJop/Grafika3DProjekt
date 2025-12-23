#include "HauntedPainting.h"


HauntedPainting::HauntedPainting(Model* model, Model* secondModel, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction), alternateModel(secondModel)
{
	alternateModel = secondModel;
}

void HauntedPainting::Interact(Inventory* playerInventory)
{
	entityModel = alternateModel;
}