#include "CandlePuzzle.h"

bool CandlePuzzle::isSolved = false;

CandlePuzzle::CandlePuzzle(std::vector<Candle*> candles, std::vector<Pedestal*> ped, Door* doors)
	: candles(candles) , doorToLock(doors), pedestals(ped) {

	for (size_t i = 0; i < pedestals.size(); i++) {
		pedestals[i]->setVisibility(false);
		pedestals[i]->setColissions(false);
		pedestals[i]->setCastsShadow(false);
		pedestals[i]->setInteractable(false);
	}

}


void CandlePuzzle::Update()
{
	if (isSolved) return;
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
				doorToLock->setLocked(true);
				doorToLock->setIsOpen(false);
				for (size_t i = 0; i < pedestals.size(); i++) {
					pedestals[i]->setVisibility(true);
					pedestals[i]->setColissions(true);
					pedestals[i]->setCastsShadow(true);
					pedestals[i]->setInteractable(true);
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