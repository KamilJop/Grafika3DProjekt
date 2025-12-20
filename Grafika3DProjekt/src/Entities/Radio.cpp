#include "Radio.h"

Radio::Radio(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	currentState = BROKEN;
	AudioManager::GetInstance().Load3DSoundEffect(brokenRadioSound,brokenRadioSound);
	AudioManager::GetInstance().Load3DSoundEffect(firstInsert, firstInsert);
	AudioManager::GetInstance().Load3DSoundEffect(secondInsert, secondInsert);
	AudioManager::GetInstance().Load3DSoundEffect(radioNoises, radioNoises);
	AudioManager::GetInstance().Load3DSoundEffect(morseCodeSound, morseCodeSound);
}

Radio::~Radio()
{
}

void Radio::Interact(Inventory* playerInventory)
{
	switch (currentState)
	{
	case BROKEN:
		if(playerInventory->GetCurrentItem()->tag == "battery1") {
			currentState = ONE_BATTERY;
			playerInventory->RemoveItem("battery1");
			AudioManager::GetInstance().Play3DSoundEffect(firstInsert, getPosition());
			UI::SetSubtitle("I inserted a battery into the radio. Maybe it will work now.", 3.0f);
		}
		else {
			AudioManager::GetInstance().Play3DSoundEffect(brokenRadioSound, getPosition());
			if(!triedToTurnOn) {
				triedToTurnOn = true;
				UI::SetSubtitle("It's dead. I need to find some batteries.", 3.0f);
			}
		}
		break;

	case ONE_BATTERY:
		if(playerInventory->GetCurrentItem()->tag == "battery2") {
			currentState = WORKING;
			playerInventory->RemoveItem("battery2");
			AudioManager::GetInstance().Play3DSoundEffect(secondInsert, getPosition());
			UI::SetSubtitle("I inserted another battery into the radio. It's working now!", 3.0f);
		}
		else {
			if (!triedToTurnOnAfterOneBattery) {
				triedToTurnOnAfterOneBattery = true;
				UI::SetSubtitle("It has one battery, but it still won't turn on. I need another one.", 3.0f);
			}
			if (soundTimer <= 0.0f) {
				soundTimer = 5.5f;
				AudioManager::GetInstance().Play3DSoundEffect(radioNoises, getPosition());
			}

		}
		break;

	case WORKING:
		if (soundTimer <= 0.0f) {
			soundTimer = 10.0f;
			AudioManager::GetInstance().Play3DSoundEffect(morseCodeSound, getPosition());
		}
		if(!playedMorse) {
			playedMorse = true;
			UI::SetSubtitle("Beeps and pauses... Is something trying to communicate through the noise?", 4.0f);
		}
		break;
	}

		
}

std::string Radio::GetActionText()
{
	switch (currentState)
	{
	case BROKEN:
		return "Turn On";
	case ONE_BATTERY:
		return "Turn On";
	case WORKING:
		return "Listen";
	default:
		return "Examine";
	}
}

void Radio::Update(float deltaTime)
{
	if (soundTimer > 0.0f) {
		soundTimer -= deltaTime;
	}
	else {
		soundTimer = 0.0f;
	}
}