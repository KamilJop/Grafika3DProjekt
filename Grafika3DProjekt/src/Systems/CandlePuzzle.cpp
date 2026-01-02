#include "CandlePuzzle.h"

bool CandlePuzzle::isSolved = false;

CandlePuzzle::CandlePuzzle(std::vector<Candle*> candles, std::vector<Candle*> candles2, std::vector<Pedestal*> ped, Door* doors)
: candles(candles), secondCandles(candles2), doorToLock(doors), pedestals(ped){

	for (size_t i = 0; i < pedestals.size(); i++) {
		pedestals[i]->setVisibility(false);
		pedestals[i]->setColissions(false);
		pedestals[i]->setCastsShadow(false);
		pedestals[i]->setInteractable(false);
	}
	secondCandles[0]->setLightPosition(secondCandles[0]->getLightPosition() + glm::vec3(0.0f, 0.0f, 0.22f));
	secondCandles[1]->setLightPosition(secondCandles[1]->getLightPosition() + glm::vec3(-0.22f, 0.0f, 0.f));
	secondCandles[2]->setLightPosition(secondCandles[2]->getLightPosition() + glm::vec3(-0.22f, 0.0f, 0.0f));
	secondCandles[3]->setLightPosition(secondCandles[3]->getLightPosition() + glm::vec3(0.0f, 0.0f, -0.22f));

	AudioManager::GetInstance().Load3DSoundEffect(demonicWhisperSoundPath, demonicWhisperSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect(doorSlamSoundPath, doorSlamSoundPath);
	AudioManager::GetInstance().Load3DSoundEffect(whooshSoundPath, whooshSoundPath);
}


void CandlePuzzle::Update(float dt)
{
	if (timer >= 6.0f) return;
	if (isSolved) {
		
		timer += dt;
		if (timer >= 1.0f) {
			secondCandles[0]->lightUp();
			if(!whooshSoundPlayed[0]) {
				whooshSoundPlayed[0] = true;
				AudioManager::GetInstance().Play3DSoundEffect(whooshSoundPath, secondCandles[0]->getPosition(), Config::getInstance().sfxVolume * 10.0f);
			}
		}

		if (timer >= 2.0f){
			secondCandles[1]->lightUp();
			if (!whooshSoundPlayed[1]) {
				whooshSoundPlayed[1] = true;
				AudioManager::GetInstance().Play3DSoundEffect(whooshSoundPath, secondCandles[1]->getPosition(), Config::getInstance().sfxVolume * 10.0f);
			}
		}
		if (timer >= 3.0f)
		{
			secondCandles[2]->lightUp();
			if (!whooshSoundPlayed[2]) {
				whooshSoundPlayed[2] = true;
				AudioManager::GetInstance().Play3DSoundEffect(whooshSoundPath, secondCandles[2]->getPosition(), Config::getInstance().sfxVolume * 10.0f);
			}

		}
		if (timer >= 4.0f) {
			secondCandles[3]->lightUp();
			if (!whooshSoundPlayed[3]) {
				whooshSoundPlayed[3] = true;
				AudioManager::GetInstance().Play3DSoundEffect(whooshSoundPath, secondCandles[3]->getPosition(), Config::getInstance().sfxVolume * 10.0f);
			}

		}
		if (timer >= 5.0f) {
			if (!doorSoundPlayed) {
				doorSoundPlayed = true;
				AudioManager::GetInstance().Play3DSoundEffect(doorSlamSoundPath, doorToLock->getPosition(), Config::getInstance().sfxVolume * 5.0f);
				doorToLock->setLocked(true);
				doorToLock->setIsOpen(false);
				doorToLock->setExamineText("Demon locked the door. I need to find a way to escape from here.");
				doorToLock->setTriedToOpen(false);
			}
		}
		return;
	}
	for (auto& candle : candles) {
		if (candle->getIsLit()) {
			std::string candleTag = candle->getTag();
			auto it = std::find(solution.begin(), solution.end(), candleTag);
			if (it == solution.end()) {
				solution.push_back(candleTag);
			}
		}
		if (solution.size() == 5) {
			bool solved = true;
			for (size_t i = 0; i < correctOrder.size(); i++) {
				if (solution[i] != correctOrder[i]) {
					solved = false;
					break;
				}
			}
			if (solved) {
				isSolved = true;
				UI::SetSubtitle("GOOD.", 3.0f);
				AudioManager::GetInstance().Play3DSoundEffect(demonicWhisperSoundPath, glm::vec3(17.0f, 0.0f, -3.0f), Config::getInstance().sfxVolume * 5.0f);
				for (size_t i = 0; i < pedestals.size(); i++) {
					pedestals[i]->setVisibility(true);
					pedestals[i]->setColissions(true);
					pedestals[i]->setCastsShadow(true);
					pedestals[i]->setInteractable(true);
				}
				for (auto& candle : candles) {
					candle->blowOut();
				}
			}
			else {
				solution.clear();
				UI::SetSubtitle("Wrong order. The candles extinguish.", 3.0f);
				for (auto& candle : candles) {
					candle->blowOut();
				}
			}

		}
	}
}