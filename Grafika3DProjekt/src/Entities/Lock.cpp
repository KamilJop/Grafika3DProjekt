#include "Lock.h"
#include "Entity.h"

Lock::Lock(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,std::vector<Model*> models, Model* metalPart,Scene* scene, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{	
	int index = 0;
	glm::vec3 posOffset = glm::vec3(0.0f);
    for (auto* m : models) {
		switch (index) {
			case 0:
				posOffset = glm::vec3(0.0375f/2.0f, 0.11725f/2.0f, 0.0f);
				break;
			case 1:
				posOffset = glm::vec3(0.0125f/2.0f, 0.11725f/2.0f, 0.0f);
				break;
			case 2:
				posOffset = glm::vec3(-0.013f/2.0f, 0.11725f/2.0f, 0.0f);
				break;
			case 3:
				posOffset = glm::vec3(-0.038f/2.0f, 0.11725f/2.0f, 0.0f);
				break;
		}
		index++;
        LockRolls.push_back(new Entity(m, pos + posOffset, rot, scal, false));
		scene->AddEntity(LockRolls.back());
    }
    metalPartModel = new Entity(metalPart, pos, rot, scal, false);
	scene->AddEntity(metalPartModel);
	lockPuzzleYaw = 90.0f;
	lockPuzzlePitch = 0.0f;
	lockPuzzlePos = pos + glm::vec3(0.0f, 0.1f, -0.25f);
	/*if (rot.y = 180.0) {
		lockPuzzlePos = pos + glm::vec3(0.0f, 0.1f, 0.5f);
		lockPuzzleYaw *= -1;
	}*/

	AudioManager::GetInstance().Load3DSoundEffect(rollSound, rollSound);
	AudioManager::GetInstance().Load3DSoundEffect(unlockSound, unlockSound);
}

void Lock::changeSelectedIndex(int direction)
{
	if (isUnlocked) return;
	LockRolls[selectedIndex]->setSelected(false);
	selectedIndex += direction;
	if (selectedIndex < 0) selectedIndex = LockRolls.size() - 1;
	if (selectedIndex >= LockRolls.size()) selectedIndex = 0;
	LockRolls[selectedIndex]->setSelected(true);
}

void Lock::moveLockRolls(int direction)
{
	if (isUnlocked) return;
	float rotationSpeed = 360.0f/7.0f * direction; 
	LockRolls[selectedIndex]->setRotation(LockRolls[selectedIndex]->getRotation() + glm::vec3(rotationSpeed, 0.0f,0.0f));
	currentAnswer[selectedIndex] += direction;
	if (currentAnswer[selectedIndex] >= 8) currentAnswer[selectedIndex] = 1;
	if (currentAnswer[selectedIndex] <= 0) currentAnswer[selectedIndex] = 7;
	AudioManager::GetInstance().Play3DSoundEffect(rollSound, getPosition(), 0.5f);
	bool correct = true;
	for (size_t i = 0; i < correctAnswer.size(); i++)
	{
		if (correctAnswer[i] != currentAnswer[i])
		{
			correct = false;
			break;
		}
	}
	if (correct)
	{
		isUnlocked = true;
		AudioManager::GetInstance().Play3DSoundEffect(unlockSound, getPosition(), 1.0f);
		if(chestToUnlock != nullptr)
		{
			chestToUnlock->OpenChest();
		}
	}
}

void Lock::cleanUpLock()
{
	for (auto* roll : LockRolls) {
		roll->setShouldGetDestroyed(true);
	}
	metalPartModel->setShouldGetDestroyed(true);
	shouldGetDestroyed = true;
}

Lock::~Lock()
{
	for (auto* roll : LockRolls) {
		roll = nullptr;
	}
	metalPartModel = nullptr;
}
