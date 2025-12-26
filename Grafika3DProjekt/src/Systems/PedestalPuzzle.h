#pragma once
#include "../Entities/Pedestal.h"
#include "../Entities/Door.h"
class PedestalPuzzle
{
public:
	PedestalPuzzle(std::vector<Pedestal*> pedestal, Door* door1, Door* door2);
	void Update();
private:
	std::vector<Pedestal*> pedestals;
	Door* doorToUnlock1;
	Door* doorToUnlock2;
};

