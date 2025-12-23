#pragma once
#include "Entity.h"
class ClockMovingPart : public Entity
{
public:
	ClockMovingPart(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = false)
		: Entity(model, pos, rot, scal, interaction)	{	}
	void Update(float deltaTime) override;
private:

	bool direction = true;
	float rotationSpeed = 25.0f;
	float minimumAngle = -10.0f;
	float maximumAngle = 30.0f;

};

