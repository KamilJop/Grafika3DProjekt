#include "Radio.h"

Radio::Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	currentState = BROKEN;
	AudioManager::GetInstance().Load3DSoundEffect(wrongFrequencySoundPath, wrongFrequencySoundPath);
}

Radio::~Radio()
{
}

void Radio::Interact(Inventory* playerInventory)
{
	switch (currentState)
	{
	case BROKEN:
		currentState = WRONG_FREQUENCY;
		break;
	case WRONG_FREQUENCY:
		AudioManager::GetInstance().Play3DSoundEffect(wrongFrequencySoundPath, position);
		break;
	}
		
}