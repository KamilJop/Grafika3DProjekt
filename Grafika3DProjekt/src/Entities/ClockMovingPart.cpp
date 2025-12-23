#include "ClockMovingPart.h"

void ClockMovingPart::Update(float deltaTime)
{
	if (direction) {
		setRotation(getRotation() + glm::vec3(0.0f, 0.0f, rotationSpeed * deltaTime));
		if (getRotation().z >= maximumAngle) {
			direction = false;
		}

		
	}
	else {
		setRotation(getRotation() - glm::vec3(0.0f, 0.0f, rotationSpeed * deltaTime));
		if (getRotation().z <= minimumAngle) {
			direction = true;
		}
	}
}