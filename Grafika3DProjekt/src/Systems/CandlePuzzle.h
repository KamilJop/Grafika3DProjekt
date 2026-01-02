#pragma once
#include "../Entities/Candle.h"
#include "../Entities/Door.h"
#include "../Entities/Pedestal.h"
#include <vector>
#include "../Core/Config.h"
#include <string>
class CandlePuzzle
{
	public:
	CandlePuzzle(std::vector<Candle*> candles,std::vector<Candle*> candles2 ,std::vector<Pedestal*> ped, Door* doors);
	void Update(float dt);
	static bool isSolved;
private:
	std::vector<Candle*> candles;
	std::vector<Candle*> secondCandles;
	std::vector<std::string> solution;
	std::vector<Pedestal*> pedestals;
	std::vector<std::string> correctOrder = { "B", "A", "E", "L", "" };
	Door* doorToLock;
	float timer = 0.0f;
	std::string demonicWhisperSoundPath = "Audio/demonic_whisper.mp3";
	std::string doorSlamSoundPath = "Audio/door_slam.mp3";
	std::string whooshSoundPath = "Audio/whoosh.mp3";
	bool doorSoundPlayed = false;
	bool whooshSoundPlayed[4] = { false, false, false, false };
};

