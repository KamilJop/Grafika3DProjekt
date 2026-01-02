#pragma once
#include "../Entities/Pedestal.h"
#include "../Entities/Door.h"
class PedestalPuzzle
{
public:
	PedestalPuzzle(std::vector<Pedestal*> pedestal, Door* door1, Door* door2);
	bool getSolved() { return puzzleSolved; }
	void Update();
private:
	std::vector<Pedestal*> pedestals;
	Door* doorToUnlock1;
	Door* doorToUnlock2;
	bool puzzleSolved = false;
	bool puzzleSolvedPlayed = false;
	std::string puzzleSolvedSoundPath = "Audio/demon_death.mp3";
};

