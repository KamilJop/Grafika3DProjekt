#include "SpinningEntity.h"
void SpinningEntity::Update(float deltaTime)  {
	if (rotation.y >= 360.0f)
		rotation.y = 0.0f;
	rotation.y += 35.0f * deltaTime;
}