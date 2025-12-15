#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include <vector>
#include "../Core/Scene.h"


class Lock : public Entity
{
public:
	Lock(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::vector<Model*>models, Model* metalPart,Scene* scene, bool interaction = true);
	void Interact(Inventory* playerInventory) override {};
	glm::vec3 getLockPuzzlePos() { return lockPuzzlePos; };
	float getLockPuzzleYaw() { return lockPuzzleYaw; };
	float getLockPuzzlePitch() { return lockPuzzlePitch; };
	int getSelectedIndex() { return selectedIndex; };
	std::vector<Entity*> getLockRolls() { return LockRolls; };
	void changeSelectedIndex(int direction);
	void moveLockRolls(int direction);
	~Lock();
private:
	std::vector<Entity*>LockRolls;
	int selectedIndex = 0;
	Entity* metalPartModel;
	bool isUnlocked = false;
	glm::vec3 lockPuzzlePos;
	float lockPuzzleYaw;
	float lockPuzzlePitch;
	std::string rollSound = "Audio/padlock_move.mp3";
};

