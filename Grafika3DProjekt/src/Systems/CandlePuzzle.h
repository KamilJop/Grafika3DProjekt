#pragma once
#include "../Entities/Candle.h"
#include "../Entities/Door.h"
#include <vector>
#include <string>
class CandlePuzzle
{
	public:
	CandlePuzzle(std::vector<Candle*> candles, Door* doors);
	void Update();
	static bool isSolved;
private:
	std::vector<Candle*> candles;
	std::vector<std::string> solution;
	std::vector<std::string> correctOrder = { "B", "A", "E", "L", "" };
	Door* doorToLock;
};

