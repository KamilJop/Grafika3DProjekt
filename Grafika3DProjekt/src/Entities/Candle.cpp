#include "Candle.h"

std::string Candle::lightingSoundPath = "Audio/candle_lighting.mp3";
std::string Candle::extinguishSoundPath = "Audio/candle_blow.mp3";


Candle::Candle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,PointLight* LightSource, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	candleLight = LightSource;
	candleLight->setPosition(pos + glm::vec3(-0.1f, 0.5f, -0.1f));
	lightAmbientIntensity = candleLight->getAmbientIntensity();
	lightDiffuseIntensity = candleLight->getDiffuseIntensity();
	candleLight->setAmbientIntensity(0.0f);
	candleLight->setDiffuseIntensity(0.0f);
	lightColor = candleLight->getColor();
	candleLight->setColor(glm::vec3(0.0f));
	AudioManager::GetInstance().Load3DSoundEffect("candle_blow", Candle::extinguishSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect("candle_lighting", Candle::lightingSoundPath);
}

Candle::~Candle()
{
}

void Candle::Interact(Inventory* playerInventory)
{	
	if (!isLit) {
		if(playerInventory->GetCurrentItem()->tag != "Lighter") {
			if(!triedToLight) {
				triedToLight = true;
				UI::SetSubtitle("I need something to light it with.", 3.0f);
			}
			return;
		}
		AudioManager::GetInstance().Play3DSoundEffect("candle_lighting", position, Config::getInstance().sfxVolume * 2.0f);
		candleLight->setAmbientIntensity(lightAmbientIntensity);
		candleLight->setDiffuseIntensity(lightDiffuseIntensity);
		candleLight->setColor(lightColor);
		isLit = true;

	}
}

void Candle::blowOut()
{
	AudioManager::GetInstance().Play3DSoundEffect("candle_blow", position);
	candleLight->setAmbientIntensity(0.0f);
	candleLight->setDiffuseIntensity(0.0f);
	candleLight->setColor(glm::vec3(0.0f));
	isLit = false;
}

void Candle::lightUp()
{	
	if (isLit) return;
	AudioManager::GetInstance().Play3DSoundEffect("candle_lighting", position, Config::getInstance().sfxVolume * 2.0f);
	candleLight->setAmbientIntensity(lightAmbientIntensity);
	candleLight->setDiffuseIntensity(lightDiffuseIntensity);
	candleLight->setColor(lightColor);
	isLit = true;
}