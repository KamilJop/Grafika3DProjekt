#include "Candle.h"

Candle::Candle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,PointLight* LightSource, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	candleLight = LightSource;
	candleLight->setPosition(pos + glm::vec3(0.0f, 0.2f, 0.0f));
	lightAmbientIntensity = candleLight->getAmbientIntensity();
	lightDiffuseIntensity = candleLight->getDiffuseIntensity();
	candleLight->setAmbientIntensity(0.0f);
	candleLight->setDiffuseIntensity(0.0f);
	lightColor = candleLight->getColor();
	candleLight->setColor(glm::vec3(0.0f));
}

Candle::~Candle()
{
}

void Candle::Interact(Inventory* playerInventory)
{
	isLit = !isLit;
	if (isLit) {
		candleLight->setAmbientIntensity(lightAmbientIntensity);
		candleLight->setDiffuseIntensity(lightDiffuseIntensity);
		candleLight->setColor(lightColor);
	}
	else {
		candleLight->setAmbientIntensity(0.0f);
		candleLight->setDiffuseIntensity(0.0f);
		candleLight->setColor(glm::vec3(0.0f));
	}
}