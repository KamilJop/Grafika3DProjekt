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
	bool getIsUnlocked() { return isUnlocked; };
	std::vector<Entity*> getLockRolls() { return LockRolls; };
	std::string GetActionText() override { return "Use"; };
	void changeSelectedIndex(int direction);
	void moveLockRolls(int direction);
	void cleanUpLock();
	~Lock();
private:
	std::vector<Entity*>LockRolls;
	std::vector<int>correctAnswer = { 5,4,3,2 };
	std::vector<int>currentAnswer = { 7,7,4,2 };
	int selectedIndex = 0;
	Entity* metalPartModel;
	bool isUnlocked = false;
	glm::vec3 lockPuzzlePos;
	float lockPuzzleYaw;
	float lockPuzzlePitch;
	std::string rollSound = "Audio/padlock_move.mp3";
	std::string unlockSound = "Audio/padlock_unlock.mp3";
};

