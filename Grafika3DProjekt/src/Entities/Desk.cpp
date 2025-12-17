#include "Desk.h"


Desk::Desk(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,std::vector<Model*> drawerModels,	Model* doorModel, Scene* scene, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	int index = 0;
	for(auto* m  : drawerModels)
	{
		std::string drawerKeyTag = "drawerKey" + std::to_string(index + 1);
		drawers.push_back(new Drawer(m, pos, rot, scal,drawerKeyTag, true));
		scene->AddEntity(drawers.back());
		drawers.back()->setTitle("Drawer");
		if (index == 2) drawers.back()->setLocked(false); 
		index++;
	}
	deskDoor = new Door(doorModel, pos + glm::vec3(-1.02f,0.6f,0.32f), rot, scal, "Desk Door", nullptr);
	deskDoor->setDirection(-1);
	scene->AddEntity(deskDoor);
}