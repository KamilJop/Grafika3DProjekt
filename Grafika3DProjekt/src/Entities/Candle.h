#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include "../Light/PointLight.h"
#include <string>
#include <vector>
class Candle : public Entity
{
	public:
	Candle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,PointLight* lightSource, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override {};
	std::string GetActionText() override { return isLit ? "Extinguish" : "Light"; };
	~Candle();
private:
	PointLight* candleLight;
	bool isLit = false;
	bool triedToLight = false;
	float lightAmbientIntensity;
	float lightDiffuseIntensity;
	glm::vec3 lightColor;
	static std::string lightingSoundPath;
	static std::string extinguishSoundPath;
};

