#pragma once
#include "../Entities/Candle.h"
#include "../Entities/Door.h"
#include "../Entities/Pedestal.h"
#include <vector>
#include <string>
class CandlePuzzle
{
	public:
	CandlePuzzle(std::vector<Candle*> candles,std::vector<Pedestal*> ped, Door* doors);
	void Update();
	static bool isSolved;
private:
	std::vector<Candle*> candles;
	std::vector<std::string> solution;
	std::vector<Pedestal*> pedestals;
	std::vector<std::string> correctOrder = { "B", "A", "E", "L", "" };
	Door* doorToLock;
};

