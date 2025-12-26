#include "PedestalPuzzle.h"

PedestalPuzzle::PedestalPuzzle(std::vector<Pedestal*> pedestal, Door* door1, Door* door2)
	: pedestals(pedestal), doorToUnlock1(door1), doorToUnlock2(door2) {
}

void PedestalPuzzle::Update()
{
	bool allCorrect = true;
	for (auto& pedestal : pedestals) {
		if (!pedestal->getHasCorrectItem()) {
			allCorrect = false;
			break;
		}
	}
	if (allCorrect) {
		doorToUnlock1->setLocked(false);
		doorToUnlock2->setLocked(false);
		doorToUnlock1->setIsOpen(true);
		doorToUnlock2->setIsOpen(true);
	}
}