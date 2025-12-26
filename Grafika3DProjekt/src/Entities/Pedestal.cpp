#include "Pedestal.h"

Pedestal::Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
}