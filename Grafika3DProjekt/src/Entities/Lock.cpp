#include "Lock.h"
#include "Entity.h"

Lock::Lock(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,std::vector<Model*> models, Model* metalPart,Scene* scene, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    for (auto* m : models) {
        LockRolls.push_back(new Entity(m, pos, rot, scal, false));
		scene->AddEntity(LockRolls.back());
    }
    metalPartModel = new Entity(metalPart, pos, rot, scal, false);
	scene->AddEntity(metalPartModel);
	lockPuzzlePos = pos + glm::vec3(0.0f, 0.1f, -0.5f);
	lockPuzzleYaw = 90.0f;
	lockPuzzlePitch = 0.0f;
	AudioManager::GetInstance().Load3DSoundEffect(rollSound, rollSound);
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
	float rotationSpeed = 45.0f * direction; 
	LockRolls[selectedIndex]->setRotation(LockRolls[selectedIndex]->getRotation() + glm::vec3(rotationSpeed, 0.0f,0.0f));
	AudioManager::GetInstance().Play3DSoundEffect(rollSound, getPosition(), 0.5f);
}
